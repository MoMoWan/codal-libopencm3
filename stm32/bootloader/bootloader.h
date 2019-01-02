//  Bootloader Functions
#ifndef BOOTLOADER_H_INCLUDED
#define BOOTLOADER_H_INCLUDED

#ifdef __cplusplus
extern "C" {  //  Allows functions below to be called by C and C++ code.
#endif

int bootloader_start(void);  //  Start the bootloader and jump to the loaded application.
int bootloader_poll(void);   //  Run bootloader in background via polling.
volatile int bootloader_status(void);  //  Return non-zero if we are receiving USB requests now.

#ifdef __cplusplus
}  //  End of extern C scope.
#endif

#endif  //  BOOTLOADER_H_INCLUDED
