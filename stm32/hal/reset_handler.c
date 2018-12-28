//  We provide our own implementation of reset_handler() so that Blue Pill bootloader and firmware will be initialised in the right sequence.
//  Based on https://github.com/libopencm3/libopencm3/blob/master/lib/cm3/vector.c
#include <libopencm3/cm3/scb.h>
#include <bootloader/bootloader.h>
#include "platform_includes.h"
#ifdef UNIT_TEST
#include <unittest/unittest.h>
#endif  //  UNIT_TEST

/* Symbols exported by the linker script(s): */
extern unsigned _data_loadaddr, _edata, _ebss;  //  For firmware rom and ram sections.
extern unsigned _boot_data_loadaddr, _boot_data, _boot_edata, _boot_ebss;  //  For bootloader rom and ram sections.
typedef void (*funcp_t) (void);
extern funcp_t __preinit_array_start, __preinit_array_end;
extern funcp_t __init_array_start, __init_array_end;
extern funcp_t __fini_array_start, __fini_array_end;

void main(void);
void blocking_handler(void);
void null_handler(void);

void pre_main() {
	//  Init the STM32 platform and start the timer.  Note: Constructors are not called yet.
    //  Note: Must disable debug when testing Deep Sleep.  Else device will not run without ST Link.
    target_enable_debug();       //  Uncomment to allow display of debug messages in development devices. NOTE: This will hang if no Arm Semihosting debugger is attached.
    //  target_disable_debug();  //  Uncomment to disable display of debug messages.  For use in production devices.
    target_init();               //  Init the STM32 platform, which calls the bootloader.  If the bootloader decides to launch the firmware, this function will not return.

#ifdef UNIT_TEST
    //  Run the unit tests if any.  Don't run unit test in bootloader, because we will run out of space in bootrom.
    run_unit_test();	
#endif  //  UNIT_TEST

    //  Start the bootloader.  This function will not return if the bootloader decides to jump to the application.
    /* int status = */
    bootloader_start();
}

void reset_handler(void) {
	//  This is called when the Blue Pill starts.  We copy the data sections from ROM to RAM, and clear the BSS sections to null.  
	//  The wrapping is done by the linker option "-Wl,-wrap,reset_handler".  The vector table points to the wrapped function.
	volatile unsigned *src, *dest, *boot_dest;
	funcp_t *fp;

	//  Copy data section from ROM to RAM.  Handle bootloader and firmware.
	for (src = &_boot_data_loadaddr, boot_dest = &_boot_data;
		boot_dest < &_boot_edata;  //  Bootloader
		src++, boot_dest++) {
		*boot_dest = *src;
	}
	for (src = &_data_loadaddr, dest = &_data;
		dest < &_edata;  //  Firmware
		src++, dest++) {
		*dest = *src;
	}

	//  Init variables in BSS section to null.  Handle bootloader and firmware.
	while (boot_dest < &_boot_ebss) { *boot_dest++ = 0; }
	while (dest < &_ebss) { *dest++ = 0; }

	/* Ensure 8-byte alignment of stack pointer on interrupts */
	/* Enabled by default on most Cortex-M parts, but not M3 r1 */
	SCB_CCR |= SCB_CCR_STKALIGN;

	//  Perform our platform initialisation.
	pre_main();

	//  Call C++ constructors for firmware.  We don't allow our low-level STM32 functions to have C++ constructors.
	//  TODO: Don't call if we are booting into bootloader, not firmware.
	/*
	for (fp = &__preinit_array_start; fp < &__preinit_array_end; fp++) {
		(*fp)();
	}
	for (fp = &__init_array_start; fp < &__init_array_end; fp++) {
		(*fp)();
	}
	*/

	/* Call the application's entry point. */
	main();

	//  Call C++ destructors.  Not used because we never return from main().
	for (fp = &__fini_array_start; fp < &__fini_array_end; fp++) {
		(*fp)();
	}

}
