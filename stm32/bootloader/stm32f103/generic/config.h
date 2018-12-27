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

#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

//  Specific settings for Blue Pill.
#define UF2_FAMILY      0x5ee21072 
#define HAVE_LED        1 
#define LED_GPIO_PORT   GPIOC 
#define LED_GPIO_PIN    GPIO13 
#define LED_OPEN_DRAIN  1 
#define USES_GPIOC      1

#define APP_BASE_ADDRESS    0x08004000 //  Firmware starts here, after the bootloader.
#define FLASH_SIZE_OVERRIDE 0x10000    //  Assume 64KB of ROM, don't override to 128 KB.
#define FLASH_PAGE_SIZE     1024
#define DFU_UPLOAD_AVAILABLE   1
#define DFU_DOWNLOAD_AVAILABLE 1

#ifndef HAVE_LED
#define HAVE_LED 0
#endif

#ifndef HAVE_BUTTON
#define HAVE_BUTTON 0
#endif

#ifndef HAVE_USB_PULLUP_CONTROL
#define HAVE_USB_PULLUP_CONTROL 0
#endif

#endif