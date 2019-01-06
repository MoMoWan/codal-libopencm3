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

static void send_hf2_response(HF2_Buffer *pkt, int size);
static void pokeSend(
    const uint8_t *dataToSend,
    volatile uint32_t dataToSendLength,
    uint8_t dataToSendFlag);
static void assert(bool assertion, const char *msg);
extern const char infoUf2File[];

//  We use a smaller buffer in Application Mode.  Enough to handle HF2_CMD_INFO, HF2_CMD_BININFO, HF2_CMD_RESET_INTO_APP, HF2_CMD_RESET_INTO_BOOTLOADER, HF2_CMD_START_FLASH.
//  Note: hf2_buffer is not initialised to 0 because it's not in the BSS section.
__attribute__ ((section(".boot_buf")))  //  Place the large packet buffer in high memory so we can reuse it as stack/heap space in Application Mode.
HF2_Buffer      hf2_buffer;             //  Large buffer for Bootloader Mode only.  Size should be 1090 bytes.
HF2_Buffer_Mini hf2_buffer_mini;        //  Small buffer for Application Mode only.  Size should be 91 bytes.
static usbd_device *_usbd_dev;
static volatile uint32_t rx_time = 0;
static uint8_t rx_buf[64];
static uint8_t tx_buf[64];
static const char bad_packet_message[] = "bad packet";

//  Remaining data to be sent.
static const uint8_t *remDataToSend;
static volatile uint32_t remDataToSendLength;
static uint8_t remDataToSendFlag = 0;
static enum StartupMode restart_request = UNKNOWN_MODE;  //  Restart after last packet has been sent.

#define checkDataSize(str, add) assert(sz == 8 + sizeof(cmd->str) + (add), "*** ERROR: checkDataSize failed")

static void handle_command(HF2_Buffer *pkt) {
	//  Must set connected flag before transmitting.
	if (!connected) {
		connected = 1;
		if (connected_func) { connected_func(); }
	}
    // one has to be careful dealing with these, as they share memory
    HF2_Command *cmd = &(pkt->cmd);
    HF2_Response *resp = &(pkt->resp);

    static uint32_t prevCmdId = 0;
    static uint32_t cmdId = 0;
    prevCmdId = cmdId;    
    cmdId = cmd->command_id;
    int sz = pkt->size;
    resp->tag = cmd->tag;
    resp->status16 = HF2_STATUS_OK;  //  Default status is OK.

    switch (cmdId) {
        case HF2_CMD_BININFO: {
            //  MakeCode sends this command first to get the mode of the device (Bootloader vs Application Mode) and flashing parameters.
            debug_println("hf2 >> bininfo");
            assert(sizeof(resp->bininfo) < HF2_MINI_BUF_SIZE, "hf2 buf too small");
            resp->bininfo.mode = (boot_target_get_startup_mode() == BOOTLOADER_MODE) ?                        
                HF2_MODE_BOOTLOADER :
                HF2_MODE_USERSPACE;
            resp->bininfo.flash_page_size = HF2_PAGE_SIZE;  //  Previously 128 * 1024
            resp->bininfo.flash_num_pages = FLASH_SIZE_OVERRIDE / HF2_PAGE_SIZE;
            resp->bininfo.max_message_size = HF2_BUF_SIZE;  //  Previously sizeof(pkt->buf);
            resp->bininfo.uf2_family = UF2_FAMILY;
            send_hf2_response(pkt, sizeof(resp->bininfo));
            debug_force_flush(); ////
            return;
        }
        case HF2_CMD_INFO: {
            //  MakeCode sends this command next to identify the device. We return the INFO_UF2.TXT file. 
            //  If device is in Application Mode, we restart to Bootloader Mode.  MakeCode sends HF2_CMD_START_FLASH before flashing.
            //  If device is in Bootloader Mode, MakeCode sends HF2_CMD_WRITE_FLASH_PAGE to flash the first page.
            debug_println("hf2 >> info");

#ifdef NOTUSED
            //  After last info packet has been sent, restart into Bootloader Mode to handle HF2_CMD_START_FLASH, which doesn't wait for restart.
            if (boot_target_get_startup_mode() == APPLICATION_MODE && 
                boot_target_get_forced_startup_mode() == UNKNOWN_MODE) {  //  If we were just forced by bootloader to restart in Application Mode, don't restart because we have just completed flashing.
                restart_request = BOOTLOADER_MODE;  //  Will restart in the tx callback after the last packet has been sent.
            }
#endif  //  NOTUSED

            int info_size = strlen(infoUf2File);
            assert(info_size > 0, "empty hf2 info");
            assert((info_size + 4) < HF2_MINI_BUF_SIZE, "hf2 buf too small");
            memcpy(pkt->resp.data8, infoUf2File, info_size);
            //  This will send first of 2 packets because info size is over 64 bytes.  The second packet will be sent by the tx callback.
            send_hf2_response(pkt, info_size);
            debug_force_flush(); ////
            return;
        }
        case HF2_CMD_START_FLASH: {
            //  Sent by MakeCode to begin flashing if we are in Bootloader Mode.
            debug_println("hf2 >> start");
            send_hf2_response(pkt, 0);
            debug_force_flush(); ////

            //  Don't allow flashing in Application Mode.  Restart now into Bootloader Mode.
            if (boot_target_get_startup_mode() == APPLICATION_MODE) { 
                boot_target_manifest_bootloader();  //  Never returns.
                return;
            }
        }
        case HF2_CMD_WRITE_FLASH_PAGE: {
            //  Sent by MakeCode to flash a single page if we are in Bootloader Mode.  We flash the page if valid.
            //  When all pages have been flashed, MakeCode sends HF2_CMD_RESET_INTO_APP to restart into Application Mode.
            uint32_t target_addr = cmd->write_flash_page.target_addr;
            const uint8_t *data = (const uint8_t *) cmd->write_flash_page.data;
            const char *valid = VALID_FLASH_ADDR(target_addr, HF2_PAGE_SIZE) ? " " : " !!! ";

            if (cmdId != prevCmdId) { debug_print("hf2 >> flash "); debug_printhex_unsigned((size_t) target_addr); debug_print(valid); }  ////
            else { debug_print(">> "); debug_printhex_unsigned((size_t) target_addr); debug_print(valid); }

            //  Don't allow flashing in Application Mode.  After last packet has been sent, restart into Bootloader Mode.
            if (boot_target_get_startup_mode() == APPLICATION_MODE) { 
                restart_request = BOOTLOADER_MODE; 
                send_hf2_response(pkt, 0);
                return;
            }

            //  First send ACK and then start writing, while getting the next packet.
            send_hf2_response(pkt, 0);

            //  Write the flash page if valid.
            checkDataSize(write_flash_page, HF2_PAGE_SIZE);
            if (VALID_FLASH_ADDR(target_addr, HF2_PAGE_SIZE)) {
#ifdef PLATFORMIO
#define FLASH_DISABLED
#endif  //  PLATFORMIO
#ifdef FLASH_DISABLED
                #warning flash_write disabled
#else
                flash_write(target_addr, data, HF2_PAGE_SIZE);
#endif  //  FLASH_DISABLED
            }
            return;
        }
        case HF2_CMD_RESET_INTO_APP: {
            //  Sent by MakeCode to restart into Application Mode after flashing.
            debug_println("hf2 >> app");
            flash_flush();  //  Flush any pending flash writes.
            restart_request = APPLICATION_MODE;

            send_hf2_response(pkt, 0);
            debug_force_flush(); ////
            return;
        }
        case HF2_CMD_RESET_INTO_BOOTLOADER: {
            //  Sent by MakeCode to restart into Bootloader Mode.
            debug_println("hf2 >> boot");
            restart_request = BOOTLOADER_MODE;

            send_hf2_response(pkt, 0);
            debug_force_flush(); ////
            return;
        }
        case HF2_CMD_READ_WORDS: {
            //  Sent by MakeCode to fetch the flash memory contents.
            debug_println("hf2 >> read");

            //  Don't allow reading in Application Mode.  After last packet has been sent, restart into Bootloader Mode.
            if (boot_target_get_startup_mode() == APPLICATION_MODE) { 
                restart_request = BOOTLOADER_MODE; 
                send_hf2_response(pkt, 0);
                return;
            }

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
                debug_println("hf2 >> murmur");
                checkDataSize(murmur3, 0);
                murmur3_core_2((void *)cmd->murmur3.target_addr, cmd->murmur3.num_words, resp->data32);
                send_hf2_response(pkt, 8);
                return;
        #endif
        default:
            // command not understood
            debug_print("hf2 >> unknown "); debug_print_unsigned(cmdId); debug_println("");
            resp->status16 = HF2_STATUS_INVALID_CMD;
            break;
    }
    //  By default send a status response with 0 data bytes.
    send_hf2_response(pkt, 0);
}

static void hf2_data_rx_cb(usbd_device *usbd_dev, uint8_t ep) {
    //  Handle the received packet.
    //  debug_print("hf2 << ep "); debug_printhex(ep); debug_println("");
    ////set_usb_busy();  //  Tell caller to repoll for USB requests.
    int len = usbd_ep_read_packet(usbd_dev, ep, rx_buf, sizeof(rx_buf));    
    rx_time = millis();
    // debug_print("hf2 << tag "); debug_printhex(rx_buf[0]); debug_println("");  // DMESG("HF2 read: %d", len);
    // dump_buffer(",", rx_buf, len);    
    if (len <= 0) return;

    uint8_t tag = rx_buf[0];
    uint8_t cmd = rx_buf[1];  //  Only valid if pkt->size = 0 (first packet of message).

    //  Use the large buffer for Bootloader Mode, small buffer for Application Mode.
    //  static HF2_Buffer *pkt = (HF2_Buffer *) &hf2_buffer_mini;
    //  static HF2_Buffer *pkt = &hf2_buffer;
    static HF2_Buffer *pkt = NULL;
    if (!pkt) {
        pkt = (boot_target_get_startup_mode() == BOOTLOADER_MODE) ?
            &hf2_buffer :
            (HF2_Buffer *) &hf2_buffer_mini;
        //  debug_print("pkt "); debug_printhex_unsigned((size_t) pkt); debug_println("");
    }

    // serial packets not allowed when in middle of command packet
    usb_assert(pkt->size == 0 || !(tag & HF2_FLAG_SERIAL_OUT), bad_packet_message);
    int size = tag & HF2_SIZE_MASK;
    usb_assert(pkt->size + size <= (int) HF2_BUF_SIZE /* Was sizeof(pkt->buf) */, bad_packet_message);

    //  Populate the packet.
    memcpy(pkt->buf + pkt->size, rx_buf + 1, size);
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
    if (remDataToSendLength > 0) {  //  If there is remaining data to be sent...
        pokeSend(remDataToSend, remDataToSendLength, remDataToSendFlag);  //  Send it.
    }
    if (remDataToSendLength == 0) {  //  If there is no more data to be sent...
        if (restart_request == BOOTLOADER_MODE) {  //  Restart to Bootloader Mode if requested.
            restart_request = UNKNOWN_MODE;
            boot_target_manifest_bootloader();  //  Never returns.
        } else if (restart_request == APPLICATION_MODE) {  //  Restart to Application Mode if requested.
            restart_request = UNKNOWN_MODE;
            boot_target_manifest_app();  //  Never returns.
        }
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
    //  TODO: Send stdout and stderr to host.
    //  HF2_FLAG_SERIAL_OUT: 0x80
    //  HF2_FLAG_SERIAL_ERR: 0xC0
	if (!connected || !usbd_dev || !buf) { return -1; }
	if (len == 0) { return 0; }

	//  Transmit Up to MAX_USB_PACKET_SIZE.
    const uint8_t *dataToSend = buf;
    uint32_t dataToSendLength = len;
    uint8_t dataToSendFlag = HF2_FLAG_CMDPKT_LAST | HF2_FLAG_SERIAL_OUT;
    memset(tx_buf, 0, sizeof(tx_buf));

    cm_disable_interrupts();
    int flag = dataToSendFlag;
    int s = sizeof(tx_buf) - 1;  //  63
    if (dataToSendLength < s) { s = dataToSendLength; }
    tx_buf[0] = flag | s;
    memcpy(tx_buf + 1, dataToSend, s);
    cm_enable_interrupts();

    //  Send the packet.
    usbd_ep_write_packet(_usbd_dev, HF2_IN, tx_buf, sizeof(tx_buf));
    dump_buffer("hf2 <<", tx_buf, s + 1);
    // debug_print("hf2ser >> "); debug_printhex(s + 1); debug_println(""); ////
    return s;
}

static void pokeSend(
    const uint8_t *dataToSend,
    volatile uint32_t dataToSendLength,
    uint8_t dataToSendFlag) {
    //  Send the next packet of the HF2 response to host.
    bool sendIt = false;
    memset(tx_buf, 0, sizeof(tx_buf));

    cm_disable_interrupts();
    int s = sizeof(tx_buf) - 1;  //  63
    if (dataToSendLength) {
        int flag = dataToSendFlag;
        if (dataToSendLength < s) {
            s = dataToSendLength;
        } else {
            if (flag == HF2_FLAG_CMDPKT_LAST)
                flag = HF2_FLAG_CMDPKT_BODY;
        }
        tx_buf[0] = flag | s;
        memcpy(tx_buf + 1, dataToSend, s);
        dataToSend += s;
        dataToSendLength -= s;
        sendIt = true;
    }
    cm_enable_interrupts();

    if (sendIt) {
        //  Send the packet.
        uint16_t len = sizeof(tx_buf);
        usbd_ep_write_packet(_usbd_dev, HF2_IN, tx_buf, len);
        //  If this message requires multiple packets, let the tx callback continue sending.
        if (dataToSendLength > 0) {
            //  Remaining data to be sent.
            remDataToSend = dataToSend;
            remDataToSendLength = dataToSendLength;
            remDataToSendFlag = dataToSendFlag;
        } else {
            remDataToSendLength = 0;  //  No more data to send.
        }
        if (s < 30) { dump_buffer("hf2 <<", tx_buf, s + 1); }
        else { debug_print("hf2 << "); debug_printhex(s + 1); debug_println(""); }
    }
}

static void hf2_set_config(usbd_device *usbd_dev, uint16_t wValue) {  (void)wValue;
    //  Setup the endpoints to be bulk & register the callbacks.
    LOG("hf2 set config");
    debug_force_flush(); ////

    usbd_ep_setup(usbd_dev, HF2_IN, USB_ENDPOINT_ATTR_BULK, MAX_USB_PACKET_SIZE, hf2_data_tx_cb);
    usbd_ep_setup(usbd_dev, HF2_OUT, USB_ENDPOINT_ATTR_BULK, MAX_USB_PACKET_SIZE, hf2_data_rx_cb);
}

void hf2_setup(usbd_device *usbd_dev, connected_callback *connected_func0) {
    //  Setup the HF2 USB interface.
    debug_print("flash allow "); debug_printhex_unsigned(USER_FLASH_START); debug_print(" to "); debug_printhex_unsigned(USER_FLASH_END); debug_println("");  //  Show the addresses that flashing is allowed.
    _usbd_dev = usbd_dev;
    connected_func = connected_func0;  //  test_hf2(); ////
    //  Note: hf2_buffer is not initialised to 0 because it's not in the BSS section.  We init here.
    if (boot_target_get_startup_mode() == BOOTLOADER_MODE) {
        memset(&hf2_buffer, 0, sizeof(hf2_buffer));
    }
    int status = aggregate_register_config_callback(usbd_dev, hf2_set_config);
    if (status < 0) { debug_println("*** hf2_setup failed"); }
}

static void assert(bool assertion, const char *msg) {
    if (assertion) { return; }
    debug_print("*** ERROR: "); debug_println(msg);
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

#endif  //  INTF_HF2

#ifdef NOTUSED

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

    if (boot_target_get_startup_mode() == APPLICATION_MODE) { return; }  //  hf2_buffer only used in Bootloader Mode.

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
