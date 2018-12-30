//  When the CODAL Application starts, we call the C++ constructors and jump to main().
//  Based on https://github.com/libopencm3/libopencm3/blob/master/lib/cm3/vector.c

extern "C" {  //  Symbols exported by the linker script(s):
	unsigned _data_loadaddr, _data, _edata, _ebss;  //  For Application rom and ram sections.
	typedef void (*funcp_t) (void);
	funcp_t __preinit_array_start, __preinit_array_end;  //  Application C++ constructors.
	funcp_t __init_array_start, __init_array_end;		 //  Application C++ constructors.
	funcp_t __fini_array_start, __fini_array_end;		 //  Application C++ destructors.
	int main(void);
};

extern "C" void application_start(void) {
	//  This is called when the Blue Pill Application starts.  We copy the data sections from ROM to RAM, and clear the BSS sections to null.  
	//  application_start() is always located at a fixed address (_text) so we can change the application easily.
	volatile unsigned *src, *dest;
	funcp_t *fp;

	//  Copy Application data section from ROM to RAM.
	for (src = &_data_loadaddr, dest = (volatile unsigned int*) &_data;
		dest < &_edata;  //  Firmware
		src++, dest++) {
		*dest = *src;
	}

	//  Init variables in Application BSS section to null.
	while (dest < &_ebss) { *dest++ = 0; }

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
