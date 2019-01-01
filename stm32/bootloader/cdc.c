//  CDC code from https://github.com/Apress/Beg-STM32-Devel-FreeRTOS-libopencm3-GCC/blob/master/rtos/usbcdcdemo/usbcdc.c
#include <libopencm3/usb/usbd.h>
#include <libopencm3/usb/cdc.h>
#include <logger.h>
#include "usb_conf.h"
#include "cdc.h"
#ifdef INTF_COMM

#define CONTROL_CALLBACK_TYPE (USB_REQ_TYPE_CLASS | USB_REQ_TYPE_INTERFACE)
#define CONTROL_CALLBACK_MASK (USB_REQ_TYPE_TYPE | USB_REQ_TYPE_RECIPIENT)
#define USB_CDC_REQ_GET_LINE_CODING		0x21

static uint8_t connected = 0;  //  Non-zero if the serial interface is connected.

//  Line config to be returned.
static const struct usb_cdc_line_coding line_coding = {
	.dwDTERate = 115200,
	.bCharFormat = USB_CDC_1_STOP_BITS,
	.bParityType = USB_CDC_NO_PARITY,
	.bDataBits = 0x08
};

static enum usbd_request_return_codes cdcacm_control_request(
  usbd_device *usbd_dev __attribute__((unused)),
  struct usb_setup_data *req,
  uint8_t **buf __attribute__((unused)),
  uint16_t *len,
  void (**complete)(
    usbd_device *usbd_dev,
    struct usb_setup_data *req
  ) __attribute__((unused))
) {
	//  Handle USB Control Requests
	//  dump_usb_request("*** cdc", req); ////
	switch (req->bRequest) {
		case USB_CDC_REQ_SET_CONTROL_LINE_STATE: {
			/* From https://github.com/libopencm3/libopencm3-examples/blob/master/examples/stm32/f3/stm32f3-discovery/usb_cdcacm/cdcacm.c
			* This Linux cdc_acm driver requires this to be implemented
			* even though it's optional in the CDC spec, and we don't
			* advertise it in the ACM functional descriptor. */
#ifdef NOTUSED
			char local_buf[10];
			struct usb_cdc_notification *notif = (void *)local_buf;

			/* We echo signals back to host as notification. */
			notif->bmRequestType = 0xA1;
			notif->bNotification = USB_CDC_NOTIFY_SERIAL_STATE;
			notif->wValue = 0;
			notif->wIndex = 0;
			notif->wLength = 2;
			local_buf[8] = req->wValue & 3;
			local_buf[9] = 0;
			// usbd_ep_write_packet(0x83, buf, 10);
#endif  //  NOTUSED
			return USBD_REQ_HANDLED;
		}
		case USB_CDC_REQ_GET_LINE_CODING: {
			//  Windows requires this request, not Mac or Linux.
			//  From https://github.com/PX4/Bootloader/blob/master/stm32/cdcacm.c
			connected = 1;
			if ( *len < sizeof(struct usb_cdc_line_coding) ) {
				debug_print("*** cdcacm_control notsupp line_coding "); debug_print_unsigned(sizeof(struct usb_cdc_line_coding)); 
				debug_print(", len "); debug_print_unsigned(*len);
				debug_println("");
				return USBD_REQ_NOTSUPP;
			}
			*buf = (uint8_t *) &line_coding;
			*len = sizeof(struct usb_cdc_line_coding);
			return USBD_REQ_HANDLED;
		}
		case USB_CDC_REQ_SET_LINE_CODING: {
			connected = 1;
			if ( *len < sizeof(struct usb_cdc_line_coding) ) {
				debug_print("*** cdcacm_control notsupp line_coding "); debug_print_unsigned(sizeof(struct usb_cdc_line_coding)); 
				debug_print(", len "); debug_print_unsigned(*len);
				debug_println("");
				return USBD_REQ_NOTSUPP;
			}
			return USBD_REQ_HANDLED;
		}
	}
	//  dump_usb_request("*** cdc next", req); ////
	return USBD_REQ_NEXT_CALLBACK;  //  Hand over to next callback.
}

static uint8_t rx_buf[MAX_USB_PACKET_SIZE + 1];   // rx buffer
// static uint8_t tx_buf[MAX_USB_PACKET_SIZE + 1];   // tx buffer

int cdcadm_transmit(
  	usbd_device *usbd_dev,
	const uint8_t *buf,
	uint16_t len
) {
	//  Send the data to the serial connection.  Send only if connected.
	if (!connected || !usbd_dev || !buf) { return -1; }
	if (len == 0) { return 0; }
	//  Transmit Up to MAX_USB_PACKET_SIZE.
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
	return len;
}

static void cdcacm_data_rx_cb(
  usbd_device *usbd_dev,
  uint8_t ep __attribute__((unused))
) {
	//  Callback when a USB packet is received.
	uint16_t len = usbd_ep_read_packet(usbd_dev, DATA_OUT, rx_buf, MAX_USB_PACKET_SIZE);
    if (len == 0) { return; }
    uint16_t pos = (len < MAX_USB_PACKET_SIZE) ? len : MAX_USB_PACKET_SIZE;
    rx_buf[pos] = 0;

	cdcadm_transmit(usbd_dev, rx_buf, pos);  //  Echo the packet.	
    //  debug_print("["); debug_println(rx_buf); debug_print("]");
}

static void cdcacm_comm_cb(
  usbd_device *usbd_dev,
  uint8_t ep __attribute__((unused))
) {
	//  Callback for the comm channel.
	debug_println("comm");
}

static void cdcacm_set_config(
  usbd_device *usbd_dev,
  uint16_t wValue __attribute__((unused))
) {
	//  Callback for setting the USB configuration.
	//  From https://github.com/libopencm3/libopencm3-examples/blob/master/examples/stm32/f3/stm32f3-discovery/usb_cdcacm/cdcacm.c
    //  debug_println("*** cdcacm_set_config"); ////
	usbd_ep_setup(usbd_dev, DATA_OUT, USB_ENDPOINT_ATTR_BULK, MAX_USB_PACKET_SIZE, cdcacm_data_rx_cb);
	usbd_ep_setup(usbd_dev, DATA_IN, USB_ENDPOINT_ATTR_BULK, MAX_USB_PACKET_SIZE, NULL);
	usbd_ep_setup(usbd_dev, COMM_IN, USB_ENDPOINT_ATTR_INTERRUPT, COMM_PACKET_SIZE, cdcacm_comm_cb);
	int status = aggregate_register_callback(
		usbd_dev,
		CONTROL_CALLBACK_TYPE,
		CONTROL_CALLBACK_MASK,
		(usbd_control_callback) cdcacm_control_request);
	if (status < 0) { debug_println("*** cdcacm_set_config failed"); debug_flush(); }
}

void cdc_setup(usbd_device* usbd_dev) {
	//  Setup the USB interface.
    //  debug_println("*** cdc_setup"); ////
	int status = aggregate_register_config_callback(usbd_dev, cdcacm_set_config);
	if (status < 0) { debug_println("*** cdc_setup failed"); debug_flush(); }
}
#endif  //  INTF_COMM
