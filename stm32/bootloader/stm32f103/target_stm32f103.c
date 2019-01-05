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

/* Common STM32F103 target functions */

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/st_usbfs.h>
#include <libopencm3/stm32/flash.h>
#include <libopencm3/stm32/desig.h>
#include <libopencm3/cm3/scb.h>
#include <logger.h>
#include "../target.h"
#include "config.h"
#include "backup.h"

#ifndef USES_GPIOA
#if (HAVE_USB_PULLUP_CONTROL == 0)
#define USES_GPIOA 1
#else
#define USES_GPIOA 0
#endif
#endif

#ifndef USES_GPIOB
#define USES_GPIOB 0
#endif

#ifndef USES_GPIOC
#define USES_GPIOC 0
#endif

static const uint32_t CMD_BOOT = 0x544F4F42UL;
static const uint32_t CMD_APP = 0x3f82722aUL;
static enum StartupMode startup_mode = UNKNOWN_MODE;

static bool validate_application(void) {
    //  Return true if there is a valid application in firmware.  The second or fourth byte should be 0xb5, the "push" instruction.
    uint32_t first_word = *(volatile uint32_t *)APP_BASE_ADDRESS;
    if ((first_word & 0xff00) == 0xb500 || (first_word & 0xff000000) == 0xb5000000) {
        debug_println("app exists");
        return true;
    }
    debug_print("app missing "); debug_printhex_unsigned((size_t) APP_BASE_ADDRESS);
    debug_print(" = "); debug_printhex_unsigned(first_word); debug_println("");
    return false;
}

enum StartupMode boot_target_get_startup_mode(void) {
    //  Get the startup mode: Bootloader or Application.
    //  return BOOTLOADER_MODE; ////
    
    if (startup_mode != UNKNOWN_MODE) { return startup_mode; }
    bool appValid = false;
    appValid = validate_application();
    if (boot_target_get_force_bootloader()) {
        //  Go to Bootloader Mode if we were requested by MakeCode to run as bootloader.
        debug_println("----bootloader mode (forced)");
        startup_mode = BOOTLOADER_MODE;
    } else if (!appValid) {
        //  Go to Bootloader Mode if no valid app exists.
        debug_println("----bootloader mode (no app)");
        startup_mode = BOOTLOADER_MODE;
    } else {
        //  Else go to Application Mode.
        debug_println("----application mode");
        startup_mode = APPLICATION_MODE;
    }
    return startup_mode;
}

void boot_target_set_led(int on) {
#if HAVE_LED
        if ((on && LED_OPEN_DRAIN) || (!on && !LED_OPEN_DRAIN)) {
            gpio_clear(LED_GPIO_PORT, LED_GPIO_PIN);
        } else {
            gpio_set(LED_GPIO_PORT, LED_GPIO_PIN);
        }
#else
    (void)on;
#endif
}

static void sleep_us(int us){
    for (int i = 0; i < us*10; i++) {
        __asm__("nop");
    }
}

void boot_target_gpio_setup(void) {
    /* Enable GPIO and USB clocks */
    rcc_periph_clock_enable(RCC_GPIOA);  //  USB on PA11, PA12.
    rcc_periph_clock_enable(RCC_GPIOB);
    rcc_periph_clock_enable(RCC_GPIOC);  //  LED on PC13.
    rcc_periph_clock_enable(RCC_USB);

    /* Setup LEDs */
#if HAVE_LED
    {
        const uint8_t mode = GPIO_MODE_OUTPUT_10_MHZ;
        const uint8_t conf = (LED_OPEN_DRAIN ? GPIO_CNF_OUTPUT_OPENDRAIN
                                             : GPIO_CNF_OUTPUT_PUSHPULL);
        if (LED_OPEN_DRAIN) {
            gpio_set(LED_GPIO_PORT, LED_GPIO_PIN);
        } else {
            gpio_clear(LED_GPIO_PORT, LED_GPIO_PIN);
        }
        gpio_set_mode(LED_GPIO_PORT, mode, conf, LED_GPIO_PIN);
    }
#endif

    /* Setup the internal pull-up/pull-down for the button */
#if HAVE_BUTTON
    {
        const uint8_t mode = GPIO_MODE_INPUT;
        const uint8_t conf = GPIO_CNF_INPUT_PULL_UPDOWN;
        gpio_set_mode(BUTTON_GPIO_PORT, mode, conf, BUTTON_GPIO_PIN);
        if (BUTTON_ACTIVE_HIGH) {
            gpio_clear(BUTTON_GPIO_PORT, BUTTON_GPIO_PIN);
        } else {
            gpio_set(BUTTON_GPIO_PORT, BUTTON_GPIO_PIN);
        }
    }
#endif

#if HAVE_USB_PULLUP_CONTROL
    {
        const uint8_t mode = GPIO_MODE_OUTPUT_10_MHZ;
        const uint8_t conf = (USB_PULLUP_OPEN_DRAIN ? GPIO_CNF_OUTPUT_OPENDRAIN
                                                    : GPIO_CNF_OUTPUT_PUSHPULL);
        /* Configure USB pullup transistor, initially disabled */
        if (USB_PULLUP_ACTIVE_HIGH) {
            gpio_clear(USB_PULLUP_GPIO_PORT, USB_PULLUP_GPIO_PIN);
        } else {
            gpio_set(USB_PULLUP_GPIO_PORT, USB_PULLUP_GPIO_PIN);
        }
        gpio_set_mode(USB_PULLUP_GPIO_PORT, mode, conf, USB_PULLUP_GPIO_PIN);
    }
#else
    {
        /* Drive the USB DP pin to override the pull-up */
        gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_10_MHZ,
                      GPIO_CNF_OUTPUT_PUSHPULL, GPIO12);
    }
#endif

    // TFT
    // RST
    gpio_clear(GPIOC, (1 << 4));
    sleep_us(20000);
    //gpio_set(GPIOC, (1 << 4));
    sleep_us(20000);
}

const usbd_driver* boot_target_usb_init(void) {
    rcc_periph_reset_pulse(RST_USB);

#if HAVE_USB_PULLUP_CONTROL
    /* Enable USB pullup to connect */
    if (USB_PULLUP_ACTIVE_HIGH) {
        gpio_set(USB_PULLUP_GPIO_PORT, USB_PULLUP_GPIO_PIN);
    } else {
        gpio_clear(USB_PULLUP_GPIO_PORT, USB_PULLUP_GPIO_PIN);
    }
#else
    /* Override hard-wired USB pullup to disconnect and reconnect */
    gpio_clear(GPIOA, GPIO12);
    int i;
    for (i = 0; i < 800000; i++) {
        __asm__("nop");
    }
#endif

    return &st_usbfs_v1_usb_driver;
}

static restart_callback_type *restart_callback_func = NULL;

void boot_target_set_restart_callback(restart_callback_type *func) {
    //  Call this function when we need to restart.  Used in Application Mode only.
    debug_print("set restart callback "); debug_printhex_unsigned((size_t) func); debug_println("");
    restart_callback_func = func;
}

void boot_target_manifest_app(void) {
    //  Restart into Application Mode to run the application.
    debug_println("boot app");
    debug_force_flush(); ////
    backup_write(BKP0, CMD_APP);
#ifdef NOTUSED
    //  In Application Mode, send a restart request so that we may flush the debug log and allow the response message to be delivered to MakeCode.
    if (restart_callback_func) { 
        restart_callback_func(); 
        return;
    }
#endif  //  NOTUSED
    scb_reset_system();  //  Otherwise restart now.
}

void boot_target_manifest_bootloader(void) {
    //  Restart into Bootloader Mode to run the bootloader.
    debug_println("boot bootloader"); debug_force_flush(); ////
    backup_write(BKP0, CMD_BOOT);

    // #define USB_CNTR_PWDN		0x0002 /* Power down */
    // #define USB_CNTR_FRES		0x0001 /* Force reset */
	// *USB_CNTR_REG |= USB_CNTR_PWDN; /* Power down */
	*USB_CNTR_REG |= USB_CNTR_FRES; /* Force reset */

    sleep_us(20000); ////

#ifdef NOTUSED
    //  In Application Mode, send a restart request so that we may flush the debug log and allow the response message to be delivered to MakeCode.
    if (restart_callback_func) { 
        restart_callback_func(); 
        return;
    }
#endif  //  NOTUSED
    scb_reset_system();  //  Otherwise restart now.
}

bool boot_target_get_force_app(void) {
    //  Return true if we should run the application at startup.
    //  Note: Should not be called twice because it changes the backup registers.
    if (backup_read(BKP0) == CMD_APP) {
        backup_write(BKP0, 0);
        return true;        
    }
    return false;
}

bool boot_target_get_force_bootloader(void) {
    //  Return true if we should run the bootloader at startup.
    //  Note: Should not be called twice because it changes the backup registers.
    bool force = false;
    /* Check the RTC backup register */
    uint32_t cmd = backup_read(BKP0);
    if (cmd == CMD_BOOT) {
        // asked to go into bootloader?
        backup_write(BKP0, 0);
        return true;
    }
    if (cmd == CMD_APP) {        
        // we were told to reset into app
        backup_write(BKP0, 0);
        return false;
    }

    // a reset now should go into app
    backup_write(BKP0, CMD_APP);

#if HAVE_BUTTON
    /* Check if the user button is held down */
    if (BUTTON_ACTIVE_HIGH) {
        if (gpio_get(BUTTON_GPIO_PORT, BUTTON_GPIO_PIN)) {
            force = true;
        }
    } else {
        if (!gpio_get(BUTTON_GPIO_PORT, BUTTON_GPIO_PIN)) {
            force = true;
        }
    }
#endif

    return force;
}

void boot_target_get_serial_number(char* dest, size_t max_chars) {
    desig_get_unique_id_as_string(dest, max_chars+1);
}

static uint16_t* get_flash_end(void) {
#ifdef FLASH_SIZE_OVERRIDE
    /* Allow access to the flash size we define */
    return (uint16_t*)(FLASH_BASE + FLASH_SIZE_OVERRIDE);
#else
    /* Only allow access to the chip's self-reported flash size */
    return (uint16_t*)(FLASH_BASE + (size_t)DESIG_FLASH_SIZE*FLASH_PAGE_SIZE);
#endif
}

size_t boot_target_get_max_firmware_size(void) {
    uint8_t* flash_end = (uint8_t*)get_flash_end();
    uint8_t* flash_start = (uint8_t*)(APP_BASE_ADDRESS);

    return (flash_end >= flash_start) ? (size_t)(flash_end - flash_start) : 0;
}

void boot_target_flash_unlock(void) {
    flash_unlock();
}

void boot_target_flash_lock(void) {
    flash_lock();
}

static inline uint16_t* get_flash_page_address(uint16_t* dest) {
    return (uint16_t*)(((uint32_t)dest / FLASH_PAGE_SIZE) * FLASH_PAGE_SIZE);
}

bool boot_target_flash_program_array(uint16_t* dest, const uint16_t* data, size_t half_word_count) {
    bool verified = true;

    /* Remember the bounds of erased data in the current page */
    static uint16_t* erase_start;
    static uint16_t* erase_end;

    const uint16_t* flash_end = get_flash_end();
    debug_print("target_flash "); debug_printhex_unsigned((size_t) dest); ////
    //  debug_print(", data "); debug_printhex_unsigned((size_t) data); 
    debug_print(" to "); debug_printhex_unsigned((size_t) flash_end); 
    debug_print(", hlen "); debug_printhex_unsigned((size_t) half_word_count); 
    debug_println(""); ////
    while (half_word_count > 0) {
        /* Avoid writing past the end of flash */
        if (dest >= flash_end) {
            //  TODO: Fails here
            debug_println("dest >= flash_end"); debug_flush();
            verified = false;
            break;
        }

        if (dest >= erase_end || dest < erase_start) {
            erase_start = get_flash_page_address(dest);
            erase_end = erase_start + (FLASH_PAGE_SIZE)/sizeof(uint16_t);
            flash_erase_page((uint32_t)erase_start);
        }
        flash_program_half_word((uint32_t)dest, *data);
        erase_start = dest + 1;
        if (*dest != *data) {
            debug_println("*dest != *data"); debug_flush();
            verified = false;
            break;
        }
        dest++;
        data++;
        half_word_count--;
    }

    return verified;
}

#ifdef NOTUSED
    #ifdef FLASH_SIZE_OVERRIDE
    _Static_assert((FLASH_BASE + FLASH_SIZE_OVERRIDE >= APP_BASE_ADDRESS),
                "Incompatible flash size");
    #endif  //  FLASH_SIZE_OVERRIDE
    
    //  #define USE_HSI 1
    void boot_target_clock_setup(void) {
    #ifdef USE_HSI
        /* Set the system clock to 48MHz from the internal RC oscillator.
        The clock tolerance doesn't meet the official USB spec, but
        it's better than nothing. */
        rcc_clock_setup_in_hsi_out_48mhz();
    #else
        /* Set system clock to 72 MHz from an external crystal */
        rcc_clock_setup_in_hse_8mhz_out_72mhz();
    #endif
    }

    void boot_target_relocate_vector_table(void) {
        SCB_VTOR = APP_BASE_ADDRESS & 0xFFFF;
    }
#endif  //  NOTUSED
