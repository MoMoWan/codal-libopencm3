#ifndef HF2_H_INCLUDED
#define HF2_H_INCLUDED
#include <libopencm3/usb/usbd.h>
#include "usb_conf.h"  //  For connected_callback

extern void hf2_setup(usbd_device *usbd_dev, connected_callback *connected_func0);
extern int hf2_transmit(
  	usbd_device *usbd_dev,
	const uint8_t *buf,
	uint16_t len
);

#endif  //  HF2_H_INCLUDED
