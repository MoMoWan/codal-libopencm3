//  Bootloader Functions
#ifndef BOOTLOADER_H_INCLUDED
#define BOOTLOADER_H_INCLUDED

#ifdef __cplusplus
extern "C" {  //  Allows functions below to be called by C and C++ code.
#endif

int bootloader_start(void);  //  Start the bootloader and jump to the loaded application.
int bootloader_poll(void);   //  Run bootloader in background via polling.
int usb_serial_transmit(     //  Transmit to the serial port, if connected.
	const uint8_t *buf,
	uint16_t len
);

#ifdef __cplusplus
}  //  End of extern C scope.
#endif

#endif  //  BOOTLOADER_H_INCLUDED
