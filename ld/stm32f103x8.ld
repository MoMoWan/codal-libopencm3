/* Linker script for STM32 Blue Pill Bootloader + Firmware: STM32F103x8, 64k flash, 20k RAM. */

/* Define memory regions. */
MEMORY
{
	/* Available ROM is 64K (0x10000). Reserve lower part for bootloader, upper part for firmware. */
	bootrom (rx)  : ORIGIN = 0x08000000,          LENGTH = 32K
	rom     (rx)  : ORIGIN = 0x08000000 + 0x8000, LENGTH = 32K

	/* Available RAM is 20K (0x5000). Reserve lower part for bootloader, upper part for firmware. */
	bootram (rwx) : ORIGIN = 0x20000000,          LENGTH = 10K
	ram     (rwx) : ORIGIN = 0x20000000 + 0x2800, LENGTH = 10K

	/* Heap and Stack start after Firmware RAM, so we should keep Bootloader RAM small */
}

/* Enforce emmition of the vector table. */
EXTERN(vector_table)

/* Define the entry point of the output file. */
ENTRY(reset_handler)

/* Define sections in the executable file, by selecting input files (.o) and archives (.a)
	`archive:file' matches file within archive 
	`archive:' matches the whole archive 
	`:file' matches file but not one in an archive */
SECTIONS
{
	/* Bootloader and Firmware ROM */

	.boot_text : {  /* Bootloader */
		*(.vectors)	/* Vector table */

		/* Bootloader Code */
		*liblibopencm3.a:          	(.text*) /* libopencm3 (CODAL build) */
		*libFrameworkLibOpenCM3.a: 	(.text*) /* libopencm3 (PlatformIO build) */
		*libunity.a:          		(.text*) /* Unit Test */
		*libqfplib.a:          		(.text*) /* Math Library */
		*libnano-float.a:          	(.text*) /* Math Library Replacement */
		*libnewlib.a:          		(.text*) /* C Library Replacement */
		*liblogger.a:          		(.text*) /* Logger */
		*libbluepill.a:          	(.text*) /* Low Level Functions */
		*libbootloader.a:          	(.text*) /* Bootloader */
		*libhal.a:          		(.text*) /* Hardware Adaptation Layer */

		. = ALIGN(4);  /* Bootloader Read-only data e.g. literal strings */
		*liblibopencm3.a:          	(.rodata*) /* libopencm3 (CODAL build) */
		*libFrameworkLibOpenCM3.a: 	(.rodata*) /* libopencm3 (PlatformIO build) */
		*libunity.a:          		(.rodata*) /* Unit Test */
		*libqfplib.a:          		(.rodata*) /* Math Library */
		*libnano-float.a:          	(.rodata*) /* Math Library Replacement */
		*libnewlib.a:          		(.rodata*) /* C Library Replacement */
		*liblogger.a:          		(.rodata*) /* Logger */
		*libbluepill.a:          	(.rodata*) /* Low Level Functions */
		*libbootloader.a:          	(.rodata*) /* Bootloader */
		*libhal.a:          		(.rodata*) /* Hardware Adaptation Layer */

		. = ALIGN(4);
	} >bootrom

	.text : {       /* Firmware */
		*(.vectors)	/* Vector table */
		*(.text*)	/* Firmware Code */
		. = ALIGN(4);
		*(.rodata*)	/* Firmware Read-only data */
		. = ALIGN(4);
	} >rom

	.boot_preinit_array : { /* Bootloader Constructors */
		. = ALIGN(4);
		__boot_preinit_array_start = .;  /*  We use KEEP to force these symbols to be linked, otherwise they will be removed because they are not referenced */
		KEEP (*liblibopencm3.a:          	(.preinit_array)) /* libopencm3 (CODAL build) */
		KEEP (*libFrameworkLibOpenCM3.a: 	(.preinit_array)) /* libopencm3 (PlatformIO build) */
		KEEP (*libunity.a:          		(.preinit_array)) /* Unit Test */
		KEEP (*libqfplib.a:          		(.preinit_array)) /* Math Library */
		KEEP (*libnano-float.a:          	(.preinit_array)) /* Math Library Replacement */
		KEEP (*libnewlib.a:          		(.preinit_array)) /* C Library Replacement */
		KEEP (*liblogger.a:          		(.preinit_array)) /* Logger */
		KEEP (*libbluepill.a:          	    (.preinit_array)) /* Low Level Functions */
		KEEP (*libbootloader.a:          	(.preinit_array)) /* Bootloader */
		KEEP (*libhal.a:          		    (.preinit_array)) /* Hardware Adaptation Layer */
		__boot_preinit_array_end = .;
	} >bootrom

	.preinit_array : { /* Firmware Constructors */
		. = ALIGN(4);
		__preinit_array_start = .;
		KEEP (*(.preinit_array))
		__preinit_array_end = .;
	} >rom

	.boot_init_array : { /* Bootloader Constructors */
		. = ALIGN(4);
		__boot_init_array_start = .;
		KEEP (*liblibopencm3.a:          	(SORT_BY_NAME (.init_array.*))) /* libopencm3 (CODAL build) */
		KEEP (*libFrameworkLibOpenCM3.a: 	(SORT_BY_NAME (.init_array.*))) /* libopencm3 (PlatformIO build) */
		KEEP (*libunity.a:          		(SORT_BY_NAME (.init_array.*))) /* Unit Test */
		KEEP (*libqfplib.a:          		(SORT_BY_NAME (.init_array.*))) /* Math Library */
		KEEP (*libnano-float.a:          	(SORT_BY_NAME (.init_array.*))) /* Math Library Replacement */
		KEEP (*libnewlib.a:          		(SORT_BY_NAME (.init_array.*))) /* C Library Replacement */
		KEEP (*liblogger.a:          		(SORT_BY_NAME (.init_array.*))) /* Logger */
		KEEP (*libbluepill.a:          	    (SORT_BY_NAME (.init_array.*))) /* Low Level Functions */
		KEEP (*libbootloader.a:          	(SORT_BY_NAME (.init_array.*))) /* Bootloader */
		KEEP (*libhal.a:          		    (SORT_BY_NAME (.init_array.*))) /* Hardware Adaptation Layer */

		KEEP (*liblibopencm3.a:          	(.init_array)) /* libopencm3 (CODAL build) */
		KEEP (*libFrameworkLibOpenCM3.a: 	(.init_array)) /* libopencm3 (PlatformIO build) */
		KEEP (*libunity.a:          		(.init_array)) /* Unit Test */
		KEEP (*libqfplib.a:          		(.init_array)) /* Math Library */
		KEEP (*libnano-float.a:          	(.init_array)) /* Math Library Replacement */
		KEEP (*libnewlib.a:          		(.init_array)) /* C Library Replacement */
		KEEP (*liblogger.a:          		(.init_array)) /* Logger */
		KEEP (*libbluepill.a:          	    (.init_array)) /* Low Level Functions */
		KEEP (*libbootloader.a:          	(.init_array)) /* Bootloader */
		KEEP (*libhal.a:          		    (.init_array)) /* Hardware Adaptation Layer */
		__boot_init_array_end = .;

		. = ALIGN(4);  /* Previously below */
		_boot_etext = .;
	} >bootrom

	.init_array : { /* Firmware Constructors */
		. = ALIGN(4);
		__init_array_start = .;
		KEEP (*(SORT_BY_NAME (.init_array.*)))
		KEEP (*(.init_array))
		__init_array_end = .;

		. = ALIGN(4);  /* Previously below */
		_etext = .;
	} >rom

	/* Not needed because we don't call C++ destructors. We provide a dummy __fini_array_start, __fini_array_end to satisfy libopencm3/vector.c */
	.fini_array : {
		. = ALIGN(4);
		__fini_array_start = .;
		/* KEEP (*(.fini_array)) */
		/* KEEP (*(SORT(.fini_array.*))) */
		__fini_array_end = .;
	} >rom

	/* Another section used by C++ stuff, appears when using newlib with 64bit (long long) printf support */
	/* Removed because we don't need C++ exception unwinding. From https://answers.launchpad.net/gcc-arm-embedded/+question/271294
		.ARM.extab : {
			*(.ARM.extab*)
		} >bootrom
		.ARM.exidx : {
			__exidx_start = .;
			*(.ARM.exidx*)
			__exidx_end = .;
		} >rom
	*/

	/* Bootloader and Firmware RAM */

	.boot_data : {          /* Bootloader */
		_boot_data = .;

		/* Bootloader read-write initialized data */
		*liblibopencm3.a:          	(.data*) /* libopencm3 (CODAL build) */
		*libFrameworkLibOpenCM3.a: 	(.data*) /* libopencm3 (PlatformIO build) */
		*libunity.a:          		(.data*) /* Unit Test */
		*libqfplib.a:          		(.data*) /* Math Library */
		*libnano-float.a:          	(.data*) /* Math Library Replacement */
		*libnewlib.a:          		(.data*) /* C Library Replacement */
		*liblogger.a:          		(.data*) /* Logger */
		*libbluepill.a:          	(.data*) /* Low Level Functions */
		*libbootloader.a:          	(.data*) /* Bootloader */
		*libhal.a:          		(.data*) /* Hardware Adaptation Layer */

		. = ALIGN(4);
		_boot_edata = .;
	} >bootram AT >bootrom  /* Stored in bootrom but copied to bootram at startup  */
	_boot_data_loadaddr = LOADADDR(.boot_data);

	.data : {        /* Firmware Data  */
		_data = .;
		*(.data*)	 /* Firmware read-write initialized data */
		. = ALIGN(4);
		_edata = .;
	} >ram AT >rom   /* Stored in rom but copied to ram at startup  */
	_data_loadaddr = LOADADDR(.data);

	.boot_bss : {   /*  Bootloader Variables */

		/*  Bootloader read-write zero initialized data */
		*liblibopencm3.a:          	(.bss*) /* libopencm3 (CODAL build) */
		*libFrameworkLibOpenCM3.a: 	(.bss*) /* libopencm3 (PlatformIO build) */
		*libunity.a:          		(.bss*) /* Unit Test */
		*libqfplib.a:          		(.bss*) /* Math Library */
		*libnano-float.a:          	(.bss*) /* Math Library Replacement */
		*libnewlib.a:          		(.bss*) /* C Library Replacement */
		*liblogger.a:          		(.bss*) /* Logger */
		*libbluepill.a:          	(.bss*) /* Low Level Functions */
		*libbootloader.a:          	(.bss*) /* Bootloader */
		*libhal.a:          		(.bss*) /* Hardware Adaptation Layer */

		*liblibopencm3.a:          	(COMMON) /* libopencm3 (CODAL build) */
		*libFrameworkLibOpenCM3.a: 	(COMMON) /* libopencm3 (PlatformIO build) */
		*libunity.a:          		(COMMON) /* Unit Test */
		*libqfplib.a:          		(COMMON) /* Math Library */
		*libnano-float.a:          	(COMMON) /* Math Library Replacement */
		*libnewlib.a:          		(COMMON) /* C Library Replacement */
		*liblogger.a:          		(COMMON) /* Logger */
		*libbluepill.a:          	(COMMON) /* Low Level Functions */
		*libbootloader.a:          	(COMMON) /* Bootloader */
		*libhal.a:          		(COMMON) /* Hardware Adaptation Layer */

		. = ALIGN(4);
		_boot_ebss = .;

		. = ALIGN(4);
		boot_end = .; /* Previously below */
	} >bootram

	.bss : {        /*  Firmware Variables  */
		*(.bss*)	/*  Read-write zero initialized data */
		*(COMMON)
		. = ALIGN(4);
		_ebss = .;

		. = ALIGN(4);
		end = .;    /* Previously below */
	} >ram

	/*
	 * The .eh_frame section appears to be used for C++ exception handling.
	 * You may need to fix this if you're using C++.
	 */
	/*  Disabled because we don't allow exceptions.
		/DISCARD/ : { *(.eh_frame) }
	*/

}

/* Heap and Stack start after Firmware RAM, so we should keep Bootloader RAM small */

PROVIDE(_stack = ORIGIN(ram) + LENGTH(ram));
