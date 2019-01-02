//  From https://github.com/mmoskal/uf2-stm32f/blob/master/hf2.c
//  #define DISABLE_DEBUG ////
#include "config.h"
#include <stdlib.h>
#include <string.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/flash.h>
#include <libopencm3/cm3/cortex.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/usb/usbd.h>
#include <libopencm3/usb/hid.h>
#include <bluepill/bluepill.h>
#include <logger/logger.h>
#include "usb_conf.h"
#include "ghostfat.h"
#include "uf2hid.h"
#include "uf2cfg.h"
#include "hf2.h"
#include "target.h"
#ifdef INTF_HF2

#define CONTROL_CALLBACK_TYPE_STANDARD (USB_REQ_TYPE_STANDARD | USB_REQ_TYPE_INTERFACE)
#define CONTROL_CALLBACK_MASK_STANDARD (USB_REQ_TYPE_TYPE | USB_REQ_TYPE_RECIPIENT)

#define CONTROL_CALLBACK_TYPE_CLASS (USB_REQ_TYPE_CLASS | USB_REQ_TYPE_INTERFACE)
#define CONTROL_CALLBACK_MASK_CLASS (USB_REQ_TYPE_TYPE | USB_REQ_TYPE_RECIPIENT)

#define VALID_FLASH_ADDR(addr, sz) (USER_FLASH_START <= (addr) && (addr) + (sz) <= USER_FLASH_END)
#define HF2_BUF_SIZE        (FLASH_PAGE_SIZE + 64)          //  Large buffer for Bootloader Mode.  Previously (1024 + 16).  Devices will typically limit it to the native flash page size + 64 bytes
#define HF2_MINI_BUF_SIZE   (4 + sizeof(UF2_INFO_TEXT))     //  Smaller buffer for Application Mode.
#define HF2_PAGE_SIZE       256                             //  MakeCode fails to flash if HF2_PAGE_SIZE is not the same as UF2 page size: U.assert(b.payloadSize == this.pageSize)
#define usb_assert          assert
#define LOG(s)              debug_println(s)

static volatile uint8_t connected = 0;  //  Non-zero if the serial interface is connected.
static connected_callback *connected_func = NULL;  //  Callback when connected.

static void test_hf2(void);

//  Large HF2 buffer for Bootloader Mode only.  Size should be 1090 bytes.
typedef struct {
    uint16_t size;
    union {
        uint8_t  buf  [HF2_BUF_SIZE];
        uint32_t buf32[HF2_BUF_SIZE / 4];
        uint16_t buf16[HF2_BUF_SIZE / 2];
        HF2_Command cmd;
        HF2_Response resp;
    } __attribute__((packed));
} __attribute__((packed)) HF2_Buffer;

//  Small HF2 buffer for Application Mode only.  Size should be 91 bytes.
typedef struct {
    uint16_t size;
    union {
        uint8_t  buf  [HF2_MINI_BUF_SIZE];
        uint32_t buf32[HF2_MINI_BUF_SIZE / 4];
        uint16_t buf16[HF2_MINI_BUF_SIZE / 2];
        HF2_Command cmd;
        HF2_Response resp;
    } __attribute__((packed));
} __attribute__((packed)) HF2_Buffer_Mini;

//  We use a smaller buffer in Application Mode.  Enough to handle HF2_CMD_INFO, HF2_CMD_BININFO, HF2_CMD_RESET_INTO_APP, HF2_CMD_RESET_INTO_BOOTLOADER, HF2_CMD_START_FLASH.
//  Note: hf2_buffer is not initialised to 0 because it's not in the BSS section.
__attribute__ ((section(".boot_buf")))  //  Place this packet buffer in high memory so it can be reused in Application Mode.
HF2_Buffer      hf2_buffer;             //  Large buffer for Bootloader Mode only.  Size should be 1090 bytes.
HF2_Buffer_Mini hf2_buffer_mini;        //  Small buffer for Application Mode only.  Size should be 91 bytes.
static usbd_device *_usbd_dev;
static volatile uint32_t rx_time = 0;

//  Remaining data to be sent.
static const uint8_t *remDataToSend;
static volatile uint32_t remDataToSendLength;
static uint8_t remDataToSendFlag;

static void pokeSend(
    const uint8_t *dataToSend,
    volatile uint32_t dataToSendLength,
    uint8_t dataToSendFlag) {
    //  Send the HF2 response to host.
    //  TODO: Send stdout and stderr to host.
    //  HF2_FLAG_SERIAL_OUT: 0x80
    //  HF2_FLAG_SERIAL_ERR: 0xC0
    //  debug_println("pokeSend"); debug_flush(); ////
    static uint8_t buf[64];
    bool sendIt = false;
    memset(buf, 0, sizeof(buf));

    cm_disable_interrupts();
    if (dataToSendLength) {
        int flag = dataToSendFlag;
        int s = 63;
        if (dataToSendLength <= 63) {
            s = dataToSendLength;
        } else {
            if (flag == HF2_FLAG_CMDPKT_LAST)
                flag = HF2_FLAG_CMDPKT_BODY;
        }

        buf[0] = flag | s;
        memcpy(buf + 1, dataToSend, s);
        dataToSend += s;
        dataToSendLength -= s;
        sendIt = true;
    }
    cm_enable_interrupts();

    if (sendIt) {
        //  Send the packet.
        uint16_t len = sizeof(buf);
        usbd_ep_write_packet(_usbd_dev, HF2_IN, buf, len);
        //  If this message requires multiple packets, let the tx callback continue sending.
        if (dataToSendLength > 0) {
            //  Remaining data to be sent.
            remDataToSend = dataToSend;
            remDataToSendLength = dataToSendLength;
            remDataToSendFlag = dataToSendFlag;
        } else {
            remDataToSendLength = 0;  //  No more data to send.
        }
        // debug_print_unsigned(millis() - rx_time); 
        dump_buffer("hf2pkt >>", buf, dataToSendLength + 1);
        // debug_print("hf2pkt >> "); debug_printhex(dataToSendLength + 1); debug_println(""); ////
    }
}

static void send_hf2_response(HF2_Buffer *pkt, int size) {
    //  Send the HF2 response message, one packet at a time.  The tx callback will send remaining packets.
    const uint8_t *dataToSend = pkt->buf;
    volatile uint32_t dataToSendLength = 4 + size;
    uint8_t dataToSendFlag = HF2_FLAG_CMDPKT_LAST;
    // dump_buffer("hf2 >>", dataToSend, size);
    pokeSend(dataToSend, dataToSendLength, dataToSendFlag);
}

int hf2_transmit(
  	usbd_device *usbd_dev,
	const uint8_t *buf,
	uint16_t len
) {
	//  Send the data to the HF2 connection.  Send only if connected.
	if (!connected || !usbd_dev || !buf) { return -1; }
	if (len == 0) { return 0; }
	//  Transmit Up to MAX_USB_PACKET_SIZE.
#ifdef TODO ////
	if (len <= MAX_USB_PACKET_SIZE) {
		return usbd_ep_write_packet(usbd_dev, DATA_IN, buf, len);  //  Returns the bytes sent.
	}
	while (len > 0) {
		uint16_t tx_len = (len > MAX_USB_PACKET_SIZE) ? MAX_USB_PACKET_SIZE : len;
		len = len - tx_len;
		uint16_t status = usbd_ep_write_packet(usbd_dev, DATA_IN, buf, tx_len);  //  Returns the bytes sent.
		if (status != tx_len) { return 0; }  //  Stop if error.
		buf = &buf[tx_len];
	}
#endif  ////  TODO
	return len;
}

extern const char infoUf2File[];

#define checkDataSize(str, add) assert(sz == 8 + sizeof(cmd->str) + (add), "*** ERROR: checkDataSize failed")

static void assert(bool assertion, const char *msg) {
    if (assertion) { return; }
    debug_print("*** ERROR: "); debug_println(msg); debug_flush();
}

static void handle_command(HF2_Buffer *pkt) {
	//  Must set connected flag before transmitting.
	if (!connected) {
		connected = 1;
		if (connected_func) { connected_func(); }
	}

    // one has to be careful dealing with these, as they share memory
    HF2_Command *cmd = &(pkt->cmd);
    HF2_Response *resp = &(pkt->resp);

    uint32_t cmdId = cmd->command_id;
    int sz = pkt->size;
    resp->tag = cmd->tag;
    resp->status16 = HF2_STATUS_OK;

#ifdef TODO
    if (timer[TIMER_BL_WAIT] < 10000)
        timer[TIMER_BL_WAIT] = 10000;
#endif  //  TODO

    switch (cmdId) {
    case HF2_CMD_INFO: {
        debug_println("hf2 info");
        int info_size = strlen(infoUf2File);
        assert(info_size > 0, "empty hf2 info");
        assert((info_size + 4) < HF2_MINI_BUF_SIZE, "hf2 buf too small");
        memcpy(pkt->resp.data8, infoUf2File, info_size);
        send_hf2_response(pkt, info_size);
        return;
    }
    case HF2_CMD_BININFO: {
        debug_println("hf2 bininfo");
        assert(sizeof(resp->bininfo) < HF2_MINI_BUF_SIZE, "hf2 buf too small");
        resp->bininfo.mode = HF2_MODE_BOOTLOADER;
        resp->bininfo.flash_page_size = HF2_PAGE_SIZE;  //  Previously 128 * 1024
        resp->bininfo.flash_num_pages = FLASH_SIZE_OVERRIDE / HF2_PAGE_SIZE;
        resp->bininfo.max_message_size = HF2_BUF_SIZE;  //  Previously sizeof(pkt->buf);
        resp->bininfo.uf2_family = UF2_FAMILY;
        send_hf2_response(pkt, sizeof(resp->bininfo));
        return;
    }
    case HF2_CMD_RESET_INTO_APP:
        //  Restart into application.
        debug_println("hf2 rst app");
        send_hf2_response(pkt, 0);
        target_manifest_app();  //  Never returns.
        return;

    case HF2_CMD_RESET_INTO_BOOTLOADER:
        //  Restart into bootloader.
        debug_println("hf2 rst boot");
        send_hf2_response(pkt, 0);
        target_manifest_bootloader();  //  Never returns.
        return;

    case HF2_CMD_START_FLASH:
        //  If we are in Application Mode, restart to Bootloader Mode.
        debug_println("hf2 start");
        send_hf2_response(pkt, 0);
        if (target_get_startup_mode() == APPLICATION_MODE) {
            target_manifest_bootloader();  //  Never returns.
        }
        return;

    case HF2_CMD_WRITE_FLASH_PAGE:
        // first send ACK and then start writing, while getting the next packet
        debug_println("hf2 write");
        checkDataSize(write_flash_page, 256);
        send_hf2_response(pkt, 0);
        if (VALID_FLASH_ADDR(cmd->write_flash_page.target_addr, 256)) {
            flash_write(cmd->write_flash_page.target_addr,
                        (const uint8_t *)cmd->write_flash_page.data, 256);
        }
        return;

    case HF2_CMD_READ_WORDS: {
        debug_println("hf2 read");
        checkDataSize(read_words, 0);
        int num_words = cmd->read_words.num_words;
        memcpy(resp->data32, (void *)cmd->read_words.target_addr, num_words << 2);
        send_hf2_response(pkt, num_words << 2);
        return;
    }

    /* TODO: Handle DMESG (0x0010)
        Return internal log buffer if any. The result is a character array.

        // no arguments
        struct HF2_DMESG_Result {
            uint8_t logs[...];
        }; */

#if MURMUR3
    case HF2_CMD_MURMUR3:
        debug_println("hf2 murmur");
        checkDataSize(murmur3, 0);
        murmur3_core_2((void *)cmd->murmur3.target_addr, cmd->murmur3.num_words, resp->data32);
        send_hf2_response(pkt, 8);
        return;
#endif
    default:
        // command not understood
        debug_print("hf2 unknown cmd "); debug_print_unsigned(cmdId); debug_println("");
        resp->status16 = HF2_STATUS_INVALID_CMD;
        break;
    }

    send_hf2_response(pkt, 0);
}

static const char bad_packet_message[] = "bad packet";
static uint8_t buf[64];

static void hf2_data_rx_cb(usbd_device *usbd_dev, uint8_t ep) {
    //  debug_print("hf2 << ep "); debug_printhex(ep); debug_println("");
    set_usb_busy();  //  Tell caller to repoll for USB requests.
    int len = usbd_ep_read_packet(usbd_dev, ep, buf, sizeof(buf));    
    rx_time = millis();
    // debug_print("hf2 << tag "); debug_printhex(buf[0]); debug_println("");  // DMESG("HF2 read: %d", len);
    // dump_buffer(",", buf, len);    
    if (len <= 0) return;

    uint8_t tag = buf[0];
    uint8_t cmd = buf[1];  //  Only valid if pkt->size = 0 (first packet of message).

    //  Use the large buffer for Bootloader Mode, small buffer for Application Mode.
    static HF2_Buffer *pkt = (HF2_Buffer *) &hf2_buffer_mini;
    ////static HF2_Buffer *pkt = &hf2_buffer;
#ifdef NOTUSED
    if (!pkt) {
        pkt = (target_get_startup_mode() == BOOTLOADER_MODE) ?
            &hf2_buffer :
            (HF2_Buffer *) &hf2_buffer_mini;
        debug_print("pkt "); debug_printhex_unsigned(pkt); debug_println("");
    }
#endif //  NOTUSED
    // serial packets not allowed when in middle of command packet
    usb_assert(pkt->size == 0 || !(tag & HF2_FLAG_SERIAL_OUT), bad_packet_message);
    int size = tag & HF2_SIZE_MASK;
    usb_assert(pkt->size + size <= (int) HF2_BUF_SIZE /* Was sizeof(pkt->buf) */, bad_packet_message);

    //  If pkt->size = 0 (first packet of message) and 
    //  cmd = BININFO (0x0001), INFO (0x0002), RESET INTO APP (0x0003), 
    //        RESET INTO (0x0004) or START FLASH (0x0005)
    //  then use smaller packet in case we are running in Application Mode.
    if ((pkt->size == 0) && 
        (tag && HF2_FLAG_CMDPKT_LAST) && 
        cmd >= 1 && cmd <= 5) {
        debug_print("small pkt "); debug_printhex(cmd);
        debug_print(", len "); debug_printhex(size);
        debug_println("");
    }
    //  Populate the packet.
    memcpy(pkt->buf + pkt->size, buf + 1, size);
    pkt->size += size;
    tag &= HF2_FLAG_MASK;
    if (tag != HF2_FLAG_CMDPKT_BODY) {
        if (tag == HF2_FLAG_CMDPKT_LAST) {
            handle_command(pkt);
        } else {
            // do something about serial?
        }
        pkt->size = 0;
    }
}

static void hf2_data_tx_cb(usbd_device *usbd_dev, uint8_t ep) { (void)usbd_dev; (void)ep;
    //  After sending a packet, send the next packet of the message.
    if (remDataToSendLength == 0) { return; }  //  No remaining data to be sent.
    set_usb_busy();  //  Tell caller to repoll for USB requests.
    pokeSend(remDataToSend, remDataToSendLength, remDataToSendFlag);
}

/** @brief Setup the endpoints to be bulk & register the callbacks. */
static void hf2_set_config(usbd_device *usbd_dev, uint16_t wValue) {
    LOG("HF2 config");
    (void)wValue;
    usbd_ep_setup(usbd_dev, HF2_IN, USB_ENDPOINT_ATTR_BULK, MAX_USB_PACKET_SIZE, hf2_data_tx_cb);
    usbd_ep_setup(usbd_dev, HF2_OUT, USB_ENDPOINT_ATTR_BULK, MAX_USB_PACKET_SIZE, hf2_data_rx_cb);
    ////connected = 1;
    ////if (connected_func) { connected_func(); }
}

void hf2_setup(usbd_device *usbd_dev, connected_callback *connected_func0) {
    _usbd_dev = usbd_dev;
    connected_func = connected_func0;

    test_hf2(); ////

    //  Note: hf2_buffer is not initialised to 0 because it's not in the BSS section.  We init here.
    if (target_get_startup_mode() == BOOTLOADER_MODE) {
        memset(&hf2_buffer, 0, sizeof(hf2_buffer));
    }
    int status = aggregate_register_config_callback(usbd_dev, hf2_set_config);
    if (status < 0) { debug_println("*** hf2_setup failed"); debug_flush(); }
}
#endif  //  INTF_HF2

static void test_hf2(void) {
    debug_print("sizeof(UF2_INFO_TEXT) ");
    debug_printhex(sizeof(UF2_INFO_TEXT));
    debug_println("");

    debug_print("infoUf2File ");
    debug_printhex_unsigned((size_t) &infoUf2File);
    debug_println("");

    debug_print("infoUf2File len ");
    debug_printhex(strlen(infoUf2File));
    debug_println("");

    if (target_get_startup_mode() == APPLICATION_MODE) { return; }  //  hf2_buffer only used in Bootloader Mode.

    debug_print("hf2_buffer ");
    debug_printhex_unsigned((size_t) &hf2_buffer);
    debug_println("");

    debug_print("*hf2_buffer before ");
    debug_printhex_unsigned(hf2_buffer.size);
    debug_println("");

    hf2_buffer.size = 0x1234;

    debug_print("*hf2_buffer after ");
    debug_printhex_unsigned(hf2_buffer.size);
    debug_println("");

    hf2_buffer.size = 0;
}

#define MURMUR3 0
#if MURMUR3
    static void murmur3_core_2(const void *data, uint32_t len, uint32_t *dst) {
        // compute two hashes with different seeds in parallel, hopefully reducing
        // collisions
        uint32_t h0 = 0x2F9BE6CC;
        uint32_t h1 = 0x1EC3A6C8;
        const uint32_t *data32 = (const uint32_t *)data;
        while (len--) {
            uint32_t k = *data32++;
            k *= 0xcc9e2d51;
            k = (k << 15) | (k >> 17);
            k *= 0x1b873593;

            h0 ^= k;
            h1 ^= k;
            h0 = (h0 << 13) | (h0 >> 19);
            h1 = (h1 << 13) | (h1 >> 19);
            h0 = (h0 * 5) + 0xe6546b64;
            h1 = (h1 * 5) + 0xe6546b64;
        }

        dst[0] = h0;
        dst[1] = h1;
    }
#endif

#ifdef NOTUSED

hf2 << len 64, tag 48, 64 / 48 01 00 00 00 5c a4 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00

48 - len 8
01 00 00 00 - cmd
5c a4 - tag
00 00 

hf2 bininfo
hf2 >> 64 / 58 5c a4 00 00 01 00 00 00 00 04 00 00 00 01 00 00 10 04 00 00 72 10 e2 5e 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00

58 - len 24
5c a4 - tag
00 - status
00 - statusinfo
01 00 00 00 - mode: bootloader
00 04 00 00 - flash_page_size: 1024 bytes
00 01 00 00 - flash_num_pages: 256
40 04 00 00 - max_message_size: 1024 + 64 = 1088
72 10 e2 5e - UF2_FAMILY 0x5ee21072

struct HF2_BININFO_Result {
    uint32_t mode;
    uint32_t flash_page_size;
    uint32_t flash_num_pages;
    uint32_t max_message_size;
    uint32_t uf2_family;
};

#endif  //  NOTUSED
