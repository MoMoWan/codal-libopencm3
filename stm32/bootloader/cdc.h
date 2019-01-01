#ifndef CDC_H_INCLUDED
#define CDC_H_INCLUDED
#include "usb_conf.h"  //  For connected_callback

//  Packet size for COMM Endpoint.  Less than the usual packet size.
#define COMM_PACKET_SIZE 16

extern void cdc_setup(usbd_device* usbd_dev, connected_callback *connected_func0);
extern int cdcadm_transmit(
  	usbd_device *usbd_dev,
	const uint8_t *buf,
	uint16_t len
);

#endif  //  CDC_H_INCLUDED
