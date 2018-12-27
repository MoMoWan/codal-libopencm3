# bootloader
`bootloader` is a MakeCode USB bootloader for STM32 Blue Pill devices that supports HF2, WebUSB and USB CDC

See https://medium.com/@ly.lee/stm32-blue-pill-usb-bootloader-how-i-fixed-the-usb-storage-serial-dfu-and-webusb-interfaces-36d7fe245b5c

https://medium.com/@ly.lee/work-in-progress-stm32-blue-pill-visual-programming-with-makecode-codal-and-libopencm3-422d308f252e

Original bootloader implementation from https://github.com/mmoskal/uf2-stm32f103

WebUSB and WinUSB implementation from https://github.com/trezor/trezor-mcu

USB CDC implementation from https://github.com/Apress/Beg-STM32-Devel-FreeRTOS-libopencm3-GCC/blob/master/rtos/usbcdcdemo/usbcdc.c

USB MSC patch from https://habr.com/company/thirdpin/blog/304924/

HF2 implementation from https://github.com/mmoskal/uf2-stm32f

Originally from https://github.com/lupyuen/bluepill-bootloader

## Build instructions

1. Launch Visual Studio Code

1. Install the PlatformIO extension for Visual Studio Code

1. Open the workspace file in the above folder

1. Build the bootloader

1. Flash the bootloader to Blue Pill via ST-Link v2

1. Browse to visualbluepill.github.io

1. Create a new project

1. At top right, click `Pair Device`. Select `DAPBoot DFU Bootloader`

1. Click `Download` to flash
