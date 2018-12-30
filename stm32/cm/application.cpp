//  When the CODAL Application starts, we call the C++ constructors and jump to main().
//  Based on https://github.com/libopencm3/libopencm3/blob/master/lib/cm3/vector.c
#include <stdint.h>
#include <logger.h>

//  Symbols exported by the linker script(s):
extern volatile uint32_t _data_loadaddr, _data, _edata, _ebss;  //  For Application rom and ram sections.
typedef void (*funcp_t) (void);
extern funcp_t __preinit_array_start, __preinit_array_end;   //  Application C++ constructors.
extern funcp_t __init_array_start, __init_array_end;		 //  Application C++ constructors.
extern funcp_t __fini_array_start, __fini_array_end;		 //  Application C++ destructors.
extern "C" int main(void);

uint32_t app_bss_test;                   //  Test whether BSS Section is loaded correctly.
uint32_t app_data_test = 0x12345678;     //  Test whether Data Section is loaded correctly.

static void pre_main() {
	//  Run some checks before we start the application.
    if (app_bss_test != 0x0 || app_data_test != 0x12345678) {
        debug_print("*** app bss/data failed, bss_test = "); debug_printhex_unsigned(app_bss_test);
        debug_print(", data_test = "); debug_printhex_unsigned(app_data_test);
        for(;;) {}
    }
}

extern "C" void application_start(void) {
	//  This is called when the Blue Pill Application starts.  We copy the data sections from ROM to RAM, and clear the BSS sections to null.  
	//  application_start() is always located at a fixed address (_text) so we can change the application easily.
	funcp_t *fp;

	//  Copy Application data section from ROM to RAM.
	volatile uint32_t *src = &_data_loadaddr;
	volatile uint32_t *dest = &_data;
	while (dest < &_edata) {
		*dest = *src;
		src = src + 1;
		dest = dest + 1;
	}

	//  Init variables in Application BSS section to null.
	while (dest < &_ebss) { *dest++ = 0; }

	//  Call any init functions here.
	pre_main();
	
	//  Call C++ constructors for Application.
	for (fp = &__preinit_array_start; fp < &__preinit_array_end; fp++) {
		(*fp)();
	}
	for (fp = &__init_array_start; fp < &__init_array_end; fp++) {
		(*fp)();
	}

	//  Call the application's entry point.
	main();

	//  Call C++ destructors.  Not used because we never return from main().
	for (fp = &__fini_array_start; fp < &__fini_array_end; fp++) {
		(*fp)();
	}
}
