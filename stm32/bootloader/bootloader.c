/*
 * Copyright (c) 2016, Devan Lai
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose with or without fee is hereby granted, provided
 * that the above copyright notice and this permission notice
 * appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 * WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
 * AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
 * LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
 * NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */


/* To enable MakeCode logging, browse to visualbluepill.github.io, open Chrome console and enter:
pxt.HF2.enableLog(); pxt.aiTrackEvent=console.log; pxt.options.debug=true
*/
//  #define DISABLE_DEBUG ////
#define EXCLUDE_PLATFORM_FUNCTIONS  //  Bootloader should not call any HAL platform functions.
#include <string.h>
#include <libopencm3/cm3/vector.h>
#include <libopencm3/cm3/scb.h>
#include <libopencm3/usb/usbd.h>
#include <libopencm3/usb/msc.h>
#include <libopencm3/stm32/gpio.h>
#include <baseloader/baseloader.h>
#include <bluepill/bluepill.h>
#include <logger/logger.h>
#include <hal/platform_includes.h>
#include "bootloader.h"
#include "target.h"
#include "usb_conf.h"
#include "dfu.h"
#include "webusb.h"
#include "winusb.h"
#include "config.h"
#include "uf2.h"
#include "backup.h"

static void poll_loop(void);
static void get_serial_number(void);

extern uint32_t _boot_stack;  //  Bootloader stack address, provided by linker script.
extern int msc_started;
static volatile int status = 0;
static volatile int last_status = 0;
static uint32_t cycleCount = 0;        
static uint32_t flushCount = 1;
static uint32_t msTimer = 0;
static usbd_device* usbd_dev = NULL;

static int baseloader_status;
static baseloader_func baseloader_addr;
static uint32_t *dest;
static const uint32_t *src;
static size_t byte_count;

static inline void __set_MSP(uint32_t topOfMainStack) {
    //  Set the stack pointer.
    asm("msr msp, %0" : : "r" (topOfMainStack));
}

int bootloader_start(void) {
    //  Start the bootloader and jump to the loaded application.
    if (usbd_dev) { return 1; }  // Already started, quit.

    debug_println("----bootloader");  // debug_flush();    
    boot_target_gpio_setup();  //  Initialize GPIO/LEDs if needed
    get_serial_number();  //  Get the unique Blue Pill serial number.

    //  If we are in Bootloader Mode, lower the stack pointer so that we can use the flash buffers in bootbuf.
    if (boot_target_get_startup_mode() == BOOTLOADER_MODE) { 
        __set_MSP((uint32_t) &_boot_stack);
    }

    //  Init the USB interfaces.
    debug_println("usb_setup");  // debug_flush();
    usbd_dev = usb_setup();

    //  If we are in Application Mode, run in background via bootloader_poll(), called every 1 millisec.
    if (boot_target_get_startup_mode() == APPLICATION_MODE) { 
        target_set_bootloader_callback(bootloader_poll);
        return 0; 
    }

#define TEST_BASELOADER
#ifdef TEST_BASELOADER
	test_copy_bootloader(); ////
	test_copy_baseloader(); ////
	// for (;;) {} ////
#endif  //  TEST_BASELOADER

#ifdef NOTUSED
	test_baseloader1(); ////
    baseloader_start();
	test_baseloader_end(); ////

	test_baseloader2(); ////
    baseloader_start();
	test_baseloader_end(); ////
#endif  //  NOTUSED

    //  Start the baseloader.  The baseloader will not return if the baseloader restarts Blue Pill after flashing.
	baseloader_addr = NULL;
	baseloader_status = baseloader_fetch(&baseloader_addr, &dest, &src, &byte_count);  //  Fetch the baseloader address, which will be at a temporary location.
	debug_print("----baseloader "); if (baseloader_status == 0) { 
		debug_printhex_unsigned((uint32_t) baseloader_addr); 
		debug_print(", dest "); debug_printhex_unsigned((uint32_t) dest);
		debug_print(", src "); debug_printhex_unsigned((uint32_t) src);
		debug_print(", len "); debug_printhex_unsigned(byte_count); debug_force_flush();  
		debug_print(", *func "); debug_printhex_unsigned(*(uint32_t *) baseloader_addr); debug_force_flush();  
	} else { debug_print_int(baseloader_status); }; debug_println(""); debug_force_flush();
	if (baseloader_status == 0 && baseloader_addr) {
		baseloader_status = baseloader_addr(dest, src, byte_count);  //  Call the baseloader.
		debug_print("baseloader failed "); debug_print_int(baseloader_status); debug_println("");  //  If it returned, it must have failed.
	}

    //  If we are in Bootloader Mode, poll forever here.
    poll_loop();
    return -1;  //  Never comes here.
}

int bootloader_poll(void) {
    //  Run bootloader in background via polling.  Return 1 if there was USB activity within the last few seconds, 0 if none.
    static uint32_t last_poll = 0;
    static uint32_t delay = 0;
    if (last_poll > 0) { delay = millis() - last_poll; } 
    last_poll = millis();

    if (!usbd_dev) { return -1; }

    //  Run any USB request processing.
	usbd_poll(usbd_dev);

    //  Get the status - should we continue polling?
    status = get_usb_status();
    //  if (status != last_status) { debug_print("@"); debug_print_unsigned(status); debug_print(" "); } ////
    last_status = status;
    return status;
    // if (delay > 0) { debug_print("p"); debug_print_unsigned(delay); debug_print(" / "); }
}

volatile int bootloader_status(void) {
    //  Return non-zero if we are receiving USB requests now.
    return get_usb_status();
}

int bootloader_set_restart_callback(restart_callback_type *func) {
    //  Call this function when we need to restart.
    boot_target_set_restart_callback(func);
    return 0;
}

static bool poll_restart_requested = false;

int poll_restart_callback(void) {
    //  Call this function when we need to restart during polling.
    debug_println("restart callback");  debug_flush(); 
    poll_restart_requested = true;
    return 0;
}

static void poll_loop(void) {
    //  Loop forever polling for USB requests.  Never returns.
    debug_println("usbd polling...");  debug_flush();  debug_flush();  // test_hf2(); test_backup();          //  Test backup.
    //  When bootloader wants to restart, wait for the context to be switched to this poll loop before flushing the log and restarting.
    boot_target_set_restart_callback(poll_restart_callback);
    while (true) {
        //  Handle the next USB request.
        usbd_poll(usbd_dev);

        //  Run some idle tasks.
        cycleCount++;
        if (cycleCount >= 700) {
            cycleCount = 0;
            boot_target_set_led((msTimer++ % 500) < 50);
#ifdef INTF_MSC
            ghostfat_1ms();  //  Handle USB storage requests.
#endif  //  INTF_MSC

            //  Flush the debug log here.  Arm Semihosting logging will interfere with USB processing.
            if (flushCount++ % 1000 == 0 && get_usb_status() == 0) {  //  If USB is not busy...
                debug_flush(); 
                if (poll_restart_requested) {  //  Flush the log and restart.
                    debug_println("restarting...");
                    debug_force_flush();

                    const int us = 1000 * 1000;
                    for (int i = 0; i < us*10; i++) { __asm__("nop"); }
                    debug_println("done");
                    debug_force_flush();

                    scb_reset_system();
                }
            }

            //  TODO: If a valid application has just been flashed, restart and run it.
        }
    }    
}

#ifdef NOTUSED
    if (appValid && !msc_started && msTimer > 1000) {
        //  If app is valid, jump to app.
        debug_println("boot_target_manifest_app");  debug_flush();
        boot_target_manifest_app();
    }
#endif // NOTUSED

static void get_serial_number(void) {
    char serial[USB_SERIAL_NUM_LENGTH+1];
    serial[0] = '\0';
    debug_println("boot_target_get_serial_number");  // debug_flush();
    boot_target_get_serial_number(serial, USB_SERIAL_NUM_LENGTH);

    debug_println("usb_set_serial_number");  // debug_flush();
    usb_set_serial_number(serial);
}

#ifdef NOTUSED
    if (appValid && boot_target_get_force_app()) {
         jump_to_application();
         return 0;
    }

    if (boot_target_get_force_bootloader() || !appValid) {    
        poll_loop();    
    } else {
        debug_println("jump_to_application");  debug_flush();
        jump_to_application();
    }    

    extern uint32_t hf2_buffer;
    extern const char infoUf2File[];

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
        debug_printhex_unsigned(hf2_buffer);
        debug_println("");

        hf2_buffer = 0x12345678;

        debug_print("*hf2_buffer after ");
        debug_printhex_unsigned(hf2_buffer);
        debug_println("");

        hf2_buffer = 0;
    }
    
    static void test_backup(void) {
        //  Test whether RTC backup registers are written correctly.
        //  static const uint32_t CMD_BOOT = 0x544F4F42UL;
        //  backup_write(BKP0, CMD_BOOT);  //  Uncomment to force booting to bootloader.

        uint32_t val = backup_read(BKP0);
        debug_print("read bkp0 "); debug_print_unsigned((size_t) val); debug_println(""); debug_flush();

        enum BackupRegister reg = BKP1;
        uint32_t cmd = backup_read(reg);
        debug_print("test_backup read "); debug_print_unsigned((size_t) cmd); debug_println(""); debug_flush();
        cmd++;
        backup_write(reg, cmd);
        debug_print("test_backup write "); debug_print_unsigned((size_t) cmd); debug_println(""); debug_flush();
        cmd = backup_read(reg);
        debug_print("test_backup read again "); debug_print_unsigned((size_t) cmd); debug_println(""); debug_flush();
    }

    static void jump_to_application(void) __attribute__ ((noreturn));

    static void jump_to_application(void) {
        //  Jump to the application main() function, which is always located at the fixed address _text according to the linker script.
        debug_print("jump to app "); debug_printhex_unsigned(APP_BASE_ADDRESS); debug_println(""); debug_flush();

        //  Fetch the application address.
        int (*application_main)() = (int (*)()) APP_BASE_ADDRESS;

        //  TODO: Initialize the application's stack pointer
        //  In Application Mode, we have more stack/heap space available because we can free up the bootloader's flashing buffers.
        //  __set_MSP((uint32_t)(app_vector_table->initial_sp_value));

        //  Jump to the address.
        //  int status = 
        (*application_main)();
        for (;;) {}  //  Should never return.
    }
#endif  //  NOTUSED
