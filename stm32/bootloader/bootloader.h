//  Bootloader Functions
#ifndef BOOTLOADER_H_INCLUDED
#define BOOTLOADER_H_INCLUDED
#include <baseloader/flash-config.h>  //  For FLASH_PAGE_SIZE

#ifdef __cplusplus
extern "C" {  //  Allows functions below to be called by C and C++ code.
#endif

extern uint8_t flashBuf[FLASH_PAGE_SIZE] __attribute__((aligned(4)));  //  Used by bootloader.c and ghostfat.c.

typedef int restart_callback_type(void);

int bootloader_start(void);  //  Start the bootloader and jump to the loaded application.
int bootloader_poll(void);   //  Run bootloader in background via polling.
volatile int bootloader_status(void);  //  Return non-zero if we are receiving USB requests now.
int bootloader_set_restart_callback(restart_callback_type *func);  //  Call this function when we need to restart.

#ifdef __cplusplus
}  //  End of extern C scope.
#endif

#endif  //  BOOTLOADER_H_INCLUDED
