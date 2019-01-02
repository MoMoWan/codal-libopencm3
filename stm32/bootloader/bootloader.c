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
#include <string.h>
#include <libopencm3/cm3/vector.h>
#include <libopencm3/usb/usbd.h>
#include <libopencm3/usb/msc.h>
#include <libopencm3/stm32/gpio.h>
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

static inline void __set_MSP(uint32_t topOfMainStack) {
    asm("msr msp, %0" : : "r" (topOfMainStack));
}

extern uint32_t _boot_stack;  //  Bootloader stack address, provided by linker script.
extern int msc_started;
static usbd_device* usbd_dev = NULL;

static void get_serial_number(void) {
    char serial[USB_SERIAL_NUM_LENGTH+1];
    serial[0] = '\0';
    debug_println("target_get_serial_number");  // debug_flush();
    target_get_serial_number(serial, USB_SERIAL_NUM_LENGTH);

    debug_println("usb_set_serial_number");  // debug_flush();
    usb_set_serial_number(serial);
}

static void poll_loop(void) {
    //  Loop forever polling for USB requests.  Never returns.
    debug_println("usbd polling...");  debug_flush();  ////
    // test_backup();          //  Test backup.
    uint32_t cycleCount = 0;        
    uint32_t flushCount = 1;
    uint32_t msTimer = 0;
    while (1) {
        cycleCount++;
        if (cycleCount >= 700) {
            msTimer++;
            cycleCount = 0;
            int v = msTimer % 500;
            target_set_led(v < 50);
#ifdef INTF_MSC
            ghostfat_1ms();
#endif  //  INTF_MSC

            //  TODO: If a valid application has just been flashed, restart and run it.
            if (flushCount++ % 50000 == 0) { debug_flush(); }  //  Must flush here.  Arm Semihosting logging will interfere with USB processing.
        }
        usbd_poll(usbd_dev);
    }    
}

#ifdef NOTUSED
    if (appValid && !msc_started && msTimer > 1000) {
        //  If app is valid, jump to app.
        debug_println("target_manifest_app");  debug_flush();
        target_manifest_app();
    }
#endif // NOTUSED

static volatile int status = 0;
static volatile int last_status = 0;

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

int bootloader_start(void) {
    //  Start the bootloader and jump to the loaded application.
    if (usbd_dev) { return 1; }  // Already started, quit.

    debug_println("----bootloader");  // debug_flush();    
    target_gpio_setup();  //  Initialize GPIO/LEDs if needed
    get_serial_number();  //  Get the unique Blue Pill serial number.
    debug_println("usb_setup");  // debug_flush();
    usbd_dev = usb_setup();

    //  If we are in Application Mode, run in background via bootloader_poll(), called every 1 millisec.
    if (target_get_startup_mode() == APPLICATION_MODE) { 
        target_set_bootloader_callback(bootloader_poll);
        return 0; 
    }
    //  If we are in Bootloader Mode, poll forever here.
    //  Lower the stack pointer so that we can use the flash buffers in bootbuf.
    __set_MSP((uint32_t) &_boot_stack);
    poll_loop();
    return -1;  //  Never comes here.
}

#ifdef NOTUSED
    if (appValid && target_get_force_app()) {
         jump_to_application();
         return 0;
    }

    if (target_get_force_bootloader() || !appValid) {    
        poll_loop();    
    } else {
        debug_println("jump_to_application");  debug_flush();
        jump_to_application();
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
