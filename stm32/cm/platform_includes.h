#ifndef PLATFORM_INCLUDES_H_
#define PLATFORM_INCLUDES_H_

#include <stdint.h>  //  For uint32_t
#include <string.h>  //  For memset()
#include <stdarg.h>  //  For va_list
#include <math.h>    //  For sin()
#include <stdlib.h>  //  For malloc()
#include "stm32.h"

#ifdef __cplusplus
extern "C" {
#endif

void target_enable_debug(void);  //  Allow display of debug messages in development devices. NOTE: This will hang if no debugger is attached.
void target_disable_debug(void);  //  Disable display of debug messages.  For use in production devices.
void target_init(void);
void target_panic(int statusCode);
void target_set_tick_callback(void (*timer_callback0)());
void target_enter_sleep_mode(void);
void target_enter_deep_sleep_stop_mode(void);
void target_enter_deep_sleep_standby_mode(void);

#ifdef __cplusplus
}
#endif

#define CODAL_ASSERT(cond)                                                                         \
    if (!(cond))                                                                                   \
    target_panic(909)

#define MBED_ASSERT CODAL_ASSERT
#define MBED_ERROR(msg) CODAL_ASSERT(0)
#define MBED_WEAK __attribute__((weak))

#ifdef PLATFORMIO  //  Define target symbols only for PlatformIO build, not Codal build.
////  TODO: Sync with target.json. Based on https://github.com/mmoskal/codal-generic-f103re/blob/master/target.json

//  Sync with linker def: ~/.platformio/packages/framework-libopencm3/lib/stm32/f1/stm32f103x8.ld
//  TODO: Should use _data and _stack but codal-core build fails because they are undefined.
//  #define DEVICE_SRAM_BASE &_data   //  Based on STM32F103C8, SRAM=0x20000000 to 0x20005000 (20KB)
//  #define DEVICE_SRAM_END  &_stack  //  Based on STM32F103C8, SRAM=0x20000000 to 0x20005000 (20KB)
#define DEVICE_SRAM_BASE 0x20000000   //  Based on STM32F103C8, SRAM=0x20000000 to 0x20005000 (20KB)
#define DEVICE_SRAM_END  0x20005000   //  Based on STM32F103C8, SRAM=0x20000000 to 0x20005000 (20KB)
#define DEVICE_STACK_BASE DEVICE_SRAM_END
#define DEVICE_STACK_SIZE 2048

#define PROCESSOR_WORD_TYPE uint32_t
#define CODAL_TIMESTAMP uint64_t
#define USB_MAX_PKT_SIZE 64
#define DEVICE_USB_ENDPOINTS 8
#define USB_DEFAULT_PID 0x2402
#define USB_DEFAULT_VID 0x03EB
#define USB_EP_FLAG_NO_AUTO_ZLP 0x01
#define TARGET_DEBUG_CLASS NOT_IMPLEMENTED
#define DEVICE_HEAP_ALLOCATOR 1
#define DEVICE_TAG 0
#define SCHEDULER_TICK_PERIOD_US 4000
#define EVENT_LISTENER_DEFAULT_FLAGS MESSAGE_BUS_LISTENER_QUEUE_IF_BUSY
#define MESSAGE_BUS_LISTENER_MAX_QUEUE_DEPTH 10
#define USE_ACCEL_LSB 0
#define DEVICE_DEFAULT_SERIAL_MODE SYNC_SLEEP
#define DEVICE_COMPONENT_COUNT 60
#define DEVICE_DEFAULT_PULLMODE PullMode::None
#define DEVICE_PANIC_HEAP_FULL 1
#define DEVICE_DMESG 1
#define DEVICE_DMESG_BUFFER_SIZE 1024
#define CODAL_DEBUG CODAL_DEBUG_DISABLED
#define DEVICE_USB 0  ////TODO: Disable flashing by USB.  Requires UF2 to be included for build.
////#define DEVICE_USB 1
#define BOOTLOADER_START_ADDR 0x00000000
#define BOOTLOADER_END_ADDR 0x00002000
#endif  //  PLATFORMIO

extern PROCESSOR_WORD_TYPE _data;   //  Start of Data segment.
extern PROCESSOR_WORD_TYPE _stack;  //  Start of Stack segment (grows downwards).

#endif  //  PLATFORM_INCLUDES_H_
