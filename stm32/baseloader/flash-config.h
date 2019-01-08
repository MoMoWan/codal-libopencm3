//  Flash Constants for Blue Pill
#ifndef FLASH_CONFIG_H_INCLUDED
#define FLASH_CONFIG_H_INCLUDED

#ifdef __cplusplus
extern "C" {  //  Allows functions below to be called by C and C++ code.
#endif

extern uint32_t _text;      //  Defined by the linker script where the application starts running.
#define APP_BASE_ADDRESS    ((uint32_t) &_text)     //  Application starts here, after the bootloader.
#define FLASH_SIZE_OVERRIDE 0x10000                 //  Assume 64KB of ROM, don't override to 128 KB.
#define FLASH_PAGE_SIZE     1024

#ifdef __cplusplus
}  //  End of extern C scope.
#endif

#endif  //  FLASH_CONFIG_H_INCLUDED
