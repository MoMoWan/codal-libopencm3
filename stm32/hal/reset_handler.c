//  We provide our own implementation of reset_handler() so that Blue Pill bootloader and firmware will be initialised in the right sequence.
//  Based on https://github.com/libopencm3/libopencm3/blob/master/lib/cm3/vector.c
#include <libopencm3/cm3/scb.h>
#include <baseloader/baseloader.h>
#include <bootloader/bootloader.h>
#include <logger.h>
#include "platform_includes.h"
#ifdef UNIT_TEST
#include <unittest/unittest.h>
#endif  //  UNIT_TEST

/* Symbols exported by the linker script(s): */
extern unsigned _boot_data_loadaddr, _boot_data, _boot_edata, _boot_ebss;  //  For bootloader rom and ram sections.
typedef void (*funcp_t) (void);
funcp_t __boot_preinit_array_start, __boot_preinit_array_end;  //  Bootloader C++ constructors.
funcp_t __boot_init_array_start, __boot_init_array_end;		 //  Bootloader C++ constructors.
funcp_t __boot_fini_array_start, __boot_fini_array_end;		 //  Bootloader C++ destructors.

void application_start(void);
void blocking_handler(void);
void null_handler(void);

typedef void (*custom_vector_table_entry_t)(void);

typedef struct {
	custom_vector_table_entry_t baseloader;
	custom_vector_table_entry_t application;
} custom_vector_table_t;

__attribute__ ((section(".custom_vectors")))
custom_vector_table_t custom_vector_table = {
	.baseloader = baseloader_start,
	.application = application_start,
};

uint32_t hal_bss_test;                   //  Test whether BSS Section is loaded correctly.
uint32_t hal_data_test = 0x87654321;     //  Test whether Data Section is loaded correctly.

static void pre_main() {
	//  Init the STM32 platform and start the timer.  Note: Constructors are not called yet.
    //  Note: Must disable debug when testing Deep Sleep.  Else device will not run without ST Link.
    target_enable_debug();       //  Uncomment to allow display of debug messages in development devices. NOTE: This will hang if no Arm Semihosting debugger is attached.
    //  target_disable_debug();  //  Uncomment to disable display of debug messages.  For use in production devices.
    target_init();               //  Init the STM32 platform.

	//  Test whether Bootloader BSS and Data Sections are loaded correctly.
    if (hal_bss_test != 0x0 || hal_data_test != 0x87654321) {
        debug_print("*** hal bss/data failed, bss_test = "); debug_printhex_unsigned(hal_bss_test);
        debug_print(", data_test = "); debug_printhex_unsigned(hal_data_test);
        for(;;) {}
    }
#ifdef UNIT_TEST
    //  Run the unit tests if any.  Don't run unit test in bootloader, because we will run out of space in bootrom.
    run_unit_test();	
#endif  //  UNIT_TEST
}

void reset_handler(void) {
	//  This is called when the Blue Pill starts.  We copy the data sections from ROM to RAM, and clear the BSS sections to null.  
	//  We also call the constructors.  The vector table points to this function.
	volatile unsigned *src, *boot_dest;
	funcp_t *fp;

	//  Copy Bootloader data section from ROM to RAM.
	for (src = &_boot_data_loadaddr, boot_dest = &_boot_data;
		boot_dest < &_boot_edata;  //  Bootloader
		src++, boot_dest++) {
		*boot_dest = *src;
	}

	//  Init variables in Bootloader BSS section to null.
	while (boot_dest < &_boot_ebss) { *boot_dest++ = 0; }

	/* Ensure 8-byte alignment of stack pointer on interrupts */
	/* Enabled by default on most Cortex-M parts, but not M3 r1 */
	SCB_CCR |= SCB_CCR_STKALIGN;

	//  Perform our platform initialisation.  pre_main() will not return if bootloader decides to run in Bootloader Mode.
	pre_main();

	//  TODO: We should not allow our low-level STM32 functions to have C++ constructors.
	//  Application constructors will be called by application_start().
	//  TODO: Fix these bootloader constructors:
	// .init_array    0x0000000008009778        0x4 .pioenvs/bluepill_f103c8/src/uart.o
 	// .init_array    0x000000000800977c        0x4 .pioenvs/bluepill_f103c8/lib33e/libuartint.a(uartint.o)
 	// .init_array    0x0000000008009780        0x4 .pioenvs/bluepill_f103c8/lib3e7/libcodal-core.a(ManagedString.o)
	for (fp = &__boot_preinit_array_start; fp < &__boot_preinit_array_end; fp++) {
		(*fp)();
	}
	for (fp = &__boot_init_array_start; fp < &__boot_init_array_end; fp++) {
		(*fp)();
	}

    //  Start the bootloader.  This function will not return if the bootloader decides to run in Bootloader Mode (polling forever for USB commands).
    bootloader_start();

	//  If we return here, that means we are running in Application Mode.
	//  Call the application's entry point. application_start() is always located at a fixed address (_text) so we can change the application easily.
	application_start();

	//  TODO: We should not allow our low-level STM32 functions to have C++ destructors.
	//  Application destructors will be called by application_start().
	for (fp = &__boot_fini_array_start; fp < &__boot_fini_array_end; fp++) {
		(*fp)();
	}

}
