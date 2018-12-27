//  We provide our own implementation of reset_handler() so that Blue Pill bootloader and firmware will be initialised in the right sequence.
//  Based on https://github.com/libopencm3/libopencm3/blob/master/lib/cm3/vector.c
#include <libopencm3/cm3/scb.h>

/* Symbols exported by the linker script(s): */
extern unsigned _data_loadaddr, _data, _edata, _ebss, _stack;
typedef void (*funcp_t) (void);
extern funcp_t __preinit_array_start, __preinit_array_end;
extern funcp_t __init_array_start, __init_array_end;
extern funcp_t __fini_array_start, __fini_array_end;

void main(void);
void blocking_handler(void);
void null_handler(void);
int run_unit_test(void);

void pre_main() {
	//  Init the STM32 platform and start the timer.
    //  Note: Must disable debug when testing Deep Sleep.  Else device will not run without ST Link.
    target_enable_debug();       //  Uncomment to allow display of debug messages in development devices. NOTE: This will hang if no Arm Semihosting debugger is attached.
    //  target_disable_debug();  //  Uncomment to disable display of debug messages.  For use in production devices.
    target_init();               //  Init the STM32 platform, which calls the bootloader.  If the bootloader decides to launch the firmware, this function will not return.

    //  Run the unit tests if any.
    run_unit_test();	
}

void __attribute__ ((naked)) reset_handler(void) {
	volatile unsigned *src, *dest;
	funcp_t *fp;

	//  Copy data section from ROM to RAM.
	//  TODO: Handle bootloader and firmware.
	for (src = &_data_loadaddr, dest = &_data;
		dest < &_edata;
		src++, dest++) {
		*dest = *src;
	}

	//  Init variables in BSS section to null.
	//  TODO: Handle bootloader and firmware.
	while (dest < &_ebss) {
		*dest++ = 0;
	}

	/* Ensure 8-byte alignment of stack pointer on interrupts */
	/* Enabled by default on most Cortex-M parts, but not M3 r1 */
	SCB_CCR |= SCB_CCR_STKALIGN;

	//  Perform our platform initialisation.
	pre_main();

	//  Call C++ constructors.
	//  TODO: Handle bootloader and firmware.
	for (fp = &__preinit_array_start; fp < &__preinit_array_end; fp++) {
		(*fp)();
	}
	for (fp = &__init_array_start; fp < &__init_array_end; fp++) {
		(*fp)();
	}

	/* Call the application's entry point. */
	main();

	//  Call C++ destructors.  Not used because we never return from main().
	for (fp = &__fini_array_start; fp < &__fini_array_end; fp++) {
		(*fp)();
	}

}
