/* Linker script for STM32 Blue Pill Bootloader + Application: STM32F103x8, 64k flash, 20k RAM. 
	Bootloader resides in low memory (always accessible) and consists of these modules:
	baseloader
	libopencm3
	cocoos
	qfplib
	nano-float
	newlib
	logger
	bluepill
	bootloader
	hal

	Application resides in high memory and consists of these modules:
	adcint
	i2cint
	porting
	simulator
	spiint
	uartint
	cm
	model
	main
	unity
	unit test 
*/

/* Define constants that may be changed to give more or less space to each region.
   TODO: Sync with ../target.json, ../target-locked.json  */

_bootrom_size = 29K;  		/* Size of Bootloader ROM (code + constant data) */

_bootram_size =  4K;  		/* Size of Bootloader RAM */

_bootbuf_size = 			/* Size of the Bootloader buffers... */
	1090 + 					/* flashBuf from stm32/bootloader/ghostfat.c */
	1024 + 					/* hf2_buffer from stm32/bootloader/hf2.c */
	   2;  					/* Align to 4 bytes */

/* Define memory regions. */
MEMORY
{
	/* Available ROM is 64K (0x10000). Reserve lower part for Bootloader, upper part for Application. */
	bootrom (rx)  : ORIGIN = 0x08000000, 				 LENGTH = _bootrom_size
	rom     (rx)  : ORIGIN = 0x08000000 + _bootrom_size, LENGTH = 64K - _bootrom_size

	/* Available RAM is 20K (0x5000). Reserve lower part for Bootloader, middle part for Application,
	   upper part for Bootloader flash buffers (not used in Application Mode). */
	bootram (rwx) : ORIGIN = 0x20000000, 				 LENGTH = _bootram_size
	ram     (rwx) : ORIGIN = 0x20000000 + _bootram_size, LENGTH = 20K - _bootram_size - _bootbuf_size
	bootbuf (rwx) : ORIGIN = 0x20005000 - _bootbuf_size, LENGTH = _bootbuf_size

	/* Heap and Stack start after the used part of Application RAM, so we should keep Bootloader RAM small */
}

/* Force vector tables to be linked in. */
EXTERN(vector_table)
EXTERN(base_vector_table)

/* Execution starts at the custom reset_handler() function in stm32/hal/reset_handler.c. */
ENTRY(reset_handler)

/* Define sections in the executable file, by selecting input files (.o) and archives (.a)
	`archive:file' matches file within archive 
	`archive:' matches the whole archive 
	`:file' matches file but not one in an archive */
SECTIONS
{
	.boot_text : {  		/* Bootloader ROM */
		_boot_text = .;				/* We will use _boot_text to determine the start of the bootloader. */

		/* Vector Tables and Baseloader Code - Must appear before others */
		*                           (.vectors) 		/* Vector Table by libopencm3.  Must appear first. */		
		*                           (.base_vectors) /* Baseloader Vector table.  Must appear second. */
		*libbaseloader.a:          	(.text*) 	   	/* Baseloader Code.  Must appear third. */
		. = ALIGN(4);
		*libbaseloader.a:          	(.rodata*) 		/* Data for Baseloader */
		. = ALIGN(4);
		_base_etext = .;							/* End of baseloader code. */

		/* List all the Interrupt Service Routines. Ensure they are operational at all times (except when Baseloader is running) */
		* 							(.text.blocking_handler*) /* Default interrupt service routines by libopencm3 */
		*		 					(.text.null_handler*) 	  /* Default interrupt service routines by libopencm3 */
		*		 					(.text.*_isr)  /* Custom interrupt service routines e.g. rtc_isr, rtc_alarm_isr */
		:*reset_handler*			(.text*) 	   /* Custom Reset Handler, also the entry point of the program */

		:*codal_target_hal*			(.text*) /* CODAL HAL */
		*libhal.a:*codal_target_hal*(.text*) /* CODAL HAL */
		:*CortexContextSwitch*	    (.text*) /* Context Switch (PlaformIO build) */
		*libhal.a:          		(.text*) /* Hardware Adaptation Layer */

		*liblibopencm3.a:          	(.text*) /* libopencm3 (CODAL build) */
		*libFrameworkLibOpenCM3.a: 	(.text*) /* libopencm3 (PlatformIO build) */
		*libcocoos.a:				(.text*) /* cocoOS (CODAL build) */
		*libcocoOS_?.?.?.a:			(.text*) /* cocoOS (PlatformIO build) */

		*libgcc.a:          		(.text*) /* C Library */
		*libqfplib.a:          		(.text*) /* Math Library */
		*libnano-float.a:          	(.text*) /* Math Library Replacement */
		*libnewlib.a:          		(.text*) /* C Library Replacement */
		*liblogger.a:          		(.text*) /* Logger */
		*libbluepill.a:          	(.text*) /* Low Level Functions */
		*libbootloader.a:          	(.text*) /* Bootloader */

		. = ALIGN(4);  /* Bootloader Read-only data e.g. literal strings */

		* 							(.rodata.blocking_handler*) /* Data for Default interrupt service routines by libopencm3 */
		*		 					(.rodata.null_handler*) 	/* Data for Default interrupt service routines by libopencm3 */
		*		 					(.rodata.*_isr)  /* Data for Custom interrupt service routines e.g. rtc_isr, rtc_alarm_isr */
		:*reset_handler*			(.rodata*) 		 /* Data for Custom Reset Handler */

		:*codal_target_hal*			(.rodata*) /* CODAL HAL */
		*libhal.a:*codal_target_hal*(.rodata*) /* CODAL HAL */
		:*CortexContextSwitch*	    (.rodata*) /* Context Switch (PlaformIO build) */
		*libhal.a:          		(.rodata*) /* Hardware Adaptation Layer */

		*liblibopencm3.a:          	(.rodata*) /* libopencm3 (CODAL build) */
		*libFrameworkLibOpenCM3.a: 	(.rodata*) /* libopencm3 (PlatformIO build) */
		*libcocoos.a:				(.rodata*) /* cocoOS (CODAL build) */
		*libcocoOS_?.?.?.a:			(.rodata*) /* cocoOS (PlatformIO build) */

		*libgcc.a:          		(.rodata*) /* C Library */
		*libqfplib.a:          		(.rodata*) /* Math Library */
		*libnano-float.a:          	(.rodata*) /* Math Library Replacement */
		*libnewlib.a:          		(.rodata*) /* C Library Replacement */
		*liblogger.a:          		(.rodata*) /* Logger */
		*libbluepill.a:          	(.rodata*) /* Low Level Functions */
		*libbootloader.a:          	(.rodata*) /* Bootloader */

		. = ALIGN(4);
		_boot_etext = .;			/* End of bootloader code. */
	} >bootrom

	.text : {       		/* Application ROM */
		. = ALIGN(4);
		_text = .;					/* We will use _text to determine where to jump to start the application. */
		* 							(.text.application_start)  /* application_start() must appear first. reset_handler() will jump here. */
		* 							(.text.main)       		   /* Followed by main() */
		* 							(.text.startup.main)       /* Sometimes main() is located in .text.startup.main */
		*							(.text*)   				   /* Other Application Code, including unit tests */

		. = ALIGN(4);
		*							(.rodata*) /* Application Read-only data */
		. = ALIGN(4);
	} >rom

	.boot_preinit_array : { /* Bootloader Constructors */
		. = ALIGN(4);
		__boot_preinit_array_start = .;  /*  We use KEEP to force these symbols to be linked, otherwise they will be removed because they are not referenced */
		/* TODO: Should not allow constructors for our low-level bootloader functions, otherwise it may cause a loop */
		KEEP (:*reset_handler*				(.preinit_array)) /* Reset Handler (CODAL build) */
		KEEP (*libhal.a:*reset_handler*		(.preinit_array)) /* Reset Handler */
		KEEP (:*codal_target_hal*			(.preinit_array)) /* CODAL HAL */
		KEEP (*libhal.a:*codal_target_hal*	(.preinit_array)) /* CODAL HAL */
		KEEP (:*CortexContextSwitch*	    (.preinit_array)) /* Context Switch (PlaformIO build) */
		KEEP (*libhal.a:          			(.preinit_array)) /* Hardware Adaptation Layer */

		KEEP (*liblibopencm3.a:          	(.preinit_array)) /* libopencm3 (CODAL build) */
		KEEP (*libFrameworkLibOpenCM3.a: 	(.preinit_array)) /* libopencm3 (PlatformIO build) */
		KEEP (*libcocoos.a:					(.preinit_array)) /* cocoOS (CODAL build) */
		KEEP (*libcocoOS_?.?.?.a:			(.preinit_array)) /* cocoOS (PlatformIO build) */

		KEEP (*libgcc.a:          			(.preinit_array)) /* C Library */
		KEEP (*libqfplib.a:          		(.preinit_array)) /* Math Library */
		KEEP (*libnano-float.a:          	(.preinit_array)) /* Math Library Replacement */
		KEEP (*libnewlib.a:          		(.preinit_array)) /* C Library Replacement */
		KEEP (*liblogger.a:          		(.preinit_array)) /* Logger */
		KEEP (*libbluepill.a:          		(.preinit_array)) /* Low Level Functions */
		KEEP (*libbootloader.a:          	(.preinit_array)) /* Bootloader */
		__boot_preinit_array_end = .;
	} >bootrom

	.preinit_array : { 		/* Application Constructors */
		. = ALIGN(4);
		__preinit_array_start = .;
		KEEP (*								(.preinit_array)) /* Application Constructors */
		__preinit_array_end = .;
	} >rom

	.boot_init_array : { 	/* Bootloader Constructors */
		. = ALIGN(4);
		__boot_init_array_start = .; 
		/* TODO: Should not allow constructors for our low-level bootloader functions, otherwise it may cause a loop */
		KEEP (:*reset_handler*				(SORT_BY_NAME (.init_array.*))) /* Reset Handler (CODAL build) */
		KEEP (*libhal.a:*reset_handler*		(SORT_BY_NAME (.init_array.*))) /* Reset Handler */
		KEEP (:*codal_target_hal*			(SORT_BY_NAME (.init_array.*))) /* CODAL HAL */
		KEEP (*libhal.a:*codal_target_hal*	(SORT_BY_NAME (.init_array.*))) /* CODAL HAL */
		KEEP (:*CortexContextSwitch*	    (SORT_BY_NAME (.init_array.*))) /* Context Switch (PlaformIO build) */
		KEEP (*libhal.a:          			(SORT_BY_NAME (.init_array.*))) /* Hardware Adaptation Layer */

		KEEP (*liblibopencm3.a:          	(SORT_BY_NAME (.init_array.*))) /* libopencm3 (CODAL build) */
		KEEP (*libFrameworkLibOpenCM3.a: 	(SORT_BY_NAME (.init_array.*))) /* libopencm3 (PlatformIO build) */
		KEEP (*libcocoos.a:					(SORT_BY_NAME (.init_array.*))) /* cocoOS (CODAL build) */
		KEEP (*libcocoOS_?.?.?.a:			(SORT_BY_NAME (.init_array.*))) /* cocoOS (PlatformIO build) */

		KEEP (*libgcc.a:          			(SORT_BY_NAME (.init_array.*))) /* C Library */
		KEEP (*libqfplib.a:          		(SORT_BY_NAME (.init_array.*))) /* Math Library */
		KEEP (*libnano-float.a:          	(SORT_BY_NAME (.init_array.*))) /* Math Library Replacement */
		KEEP (*libnewlib.a:          		(SORT_BY_NAME (.init_array.*))) /* C Library Replacement */
		KEEP (*liblogger.a:          		(SORT_BY_NAME (.init_array.*))) /* Logger */
		KEEP (*libbluepill.a:          		(SORT_BY_NAME (.init_array.*))) /* Low Level Functions */
		KEEP (*libbootloader.a:          	(SORT_BY_NAME (.init_array.*))) /* Bootloader */

		KEEP (:*reset_handler*				(.init_array)) /* Reset Handler (CODAL build) */
		KEEP (*libhal.a:*reset_handler*		(.init_array)) /* Reset Handler */
		KEEP (:*codal_target_hal*			(.init_array)) /* CODAL HAL */
		KEEP (*libhal.a:*codal_target_hal*	(.init_array)) /* CODAL HAL */
		KEEP (:*CortexContextSwitch*	    (.init_array)) /* Context Switch (PlaformIO build) */
		KEEP (*libhal.a:          			(.init_array)) /* Hardware Adaptation Layer */

		KEEP (*liblibopencm3.a:          	(.init_array)) /* libopencm3 (CODAL build) */
		KEEP (*libFrameworkLibOpenCM3.a: 	(.init_array)) /* libopencm3 (PlatformIO build) */
		KEEP (*libcocoos.a:					(.init_array)) /* cocoOS (CODAL build) */
		KEEP (*libcocoOS_?.?.?.a:			(.init_array)) /* cocoOS (PlatformIO build) */

		KEEP (*libgcc.a:          			(.init_array)) /* C Library */
		KEEP (*libqfplib.a:          		(.init_array)) /* Math Library */
		KEEP (*libnano-float.a:          	(.init_array)) /* Math Library Replacement */
		KEEP (*libnewlib.a:          		(.init_array)) /* C Library Replacement */
		KEEP (*liblogger.a:          		(.init_array)) /* Logger */
		KEEP (*libbluepill.a:          		(.init_array)) /* Low Level Functions */
		KEEP (*libbootloader.a:          	(.init_array)) /* Bootloader */
		__boot_init_array_end = .;
		. = ALIGN(4);  /* Previously below */
		_boot_etext = .;
	} >bootrom

	.init_array : { 		/* Application Constructors */
		. = ALIGN(4);
		__init_array_start = .;
		KEEP (*					(SORT_BY_NAME (.init_array.*))) /* Application Constructors. */
		KEEP (*					(.init_array)) 					/* Application Constructors. */
		__init_array_end = .;

		. = ALIGN(4);  /* Previously below */
		_etext = .;
	} >rom

	.boot_fini_array : { 	/* Bootloader Destructors. There should be no destructors in Bootloader. */
		. = ALIGN(4);
		__boot_fini_array_start = .;
		KEEP (:*reset_handler*				(.fini_array)) /* Reset Handler (CODAL build) */
		KEEP (*libhal.a:*reset_handler*		(.fini_array)) /* Reset Handler */
		KEEP (:*codal_target_hal*			(.fini_array)) /* CODAL HAL */
		KEEP (*libhal.a:*codal_target_hal*	(.fini_array)) /* CODAL HAL */
		KEEP (:*CortexContextSwitch*	    (.fini_array)) /* Context Switch (PlaformIO build) */
		KEEP (*libhal.a:          			(.fini_array)) /* Hardware Adaptation Layer */

		KEEP (*liblibopencm3.a:          	(.fini_array)) /* libopencm3 (CODAL build) */
		KEEP (*libFrameworkLibOpenCM3.a: 	(.fini_array)) /* libopencm3 (PlatformIO build) */
		KEEP (*libcocoos.a:					(.fini_array)) /* cocoOS (CODAL build) */
		KEEP (*libcocoOS_?.?.?.a:			(.fini_array)) /* cocoOS (PlatformIO build) */

		KEEP (*libgcc.a:          			(.fini_array)) /* C Library */
		KEEP (*libqfplib.a:          		(.fini_array)) /* Math Library */
		KEEP (*libnano-float.a:          	(.fini_array)) /* Math Library Replacement */
		KEEP (*libnewlib.a:          		(.fini_array)) /* C Library Replacement */
		KEEP (*liblogger.a:          		(.fini_array)) /* Logger */
		KEEP (*libbluepill.a:          		(.fini_array)) /* Low Level Functions */
		KEEP (*libbootloader.a:          	(.fini_array)) /* Bootloader */

		KEEP (:*reset_handler*				(SORT (.fini_array.*))) /* Reset Handler (CODAL build) */
		KEEP (*libhal.a:*reset_handler*		(SORT (.fini_array.*))) /* Reset Handler */
		KEEP (:*codal_target_hal*			(SORT (.fini_array.*))) /* CODAL HAL */
		KEEP (*libhal.a:*codal_target_hal*	(SORT (.fini_array.*))) /* CODAL HAL */
		KEEP (:*CortexContextSwitch*	    (SORT (.fini_array.*))) /* Context Switch (PlaformIO build) */
		KEEP (*libhal.a:          			(SORT (.fini_array.*))) /* Hardware Adaptation Layer */

		KEEP (*liblibopencm3.a:          	(SORT (.fini_array.*))) /* libopencm3 (CODAL build) */
		KEEP (*libFrameworkLibOpenCM3.a: 	(SORT (.fini_array.*))) /* libopencm3 (PlatformIO build) */
		KEEP (*libcocoos.a:					(SORT (.fini_array.*))) /* cocoOS (CODAL build) */
		KEEP (*libcocoOS_?.?.?.a:			(SORT (.fini_array.*))) /* cocoOS (PlatformIO build) */

		KEEP (*libgcc.a:          			(SORT (.fini_array.*))) /* C Library */
		KEEP (*libqfplib.a:          		(SORT (.fini_array.*))) /* Math Library */
		KEEP (*libnano-float.a:          	(SORT (.fini_array.*))) /* Math Library Replacement */
		KEEP (*libnewlib.a:          		(SORT (.fini_array.*))) /* C Library Replacement */
		KEEP (*liblogger.a:          		(SORT (.fini_array.*))) /* Logger */
		KEEP (*libbluepill.a:          		(SORT (.fini_array.*))) /* Low Level Functions */
		KEEP (*libbootloader.a:          	(SORT (.fini_array.*))) /* Bootloader */
		__boot_fini_array_end = .;
	} >bootrom

	.fini_array : { 		/* Application Destructors. */
		. = ALIGN(4);
		__fini_array_start = .;
		KEEP (*					(.fini_array))         /* Application Destructors. */
		KEEP (*					(SORT(.fini_array.*))) /* Application Destructors. */
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

	/* Bootloader and Application RAM */

	.boot_data : {          /* Bootloader RAM */
		_boot_data = .;

		/* Bootloader read-write initialized data */
		*                           (.base_para) /* Baseloader Parameters.  Must appear first. */
		*                           (.base_tmp) /* Baseloader Temporary Vars.  Must appear next. */
		*libbaseloader.a:          	(.data*) /* Baseloader */
		:*reset_handler*			(.data*) /* Reset Handler */
		*libhal.a:*reset_handler*	(.data*) /* Reset Handler */
		:*codal_target_hal*			(.data*) /* CODAL HAL */
		*libhal.a:*codal_target_hal*(.data*) /* CODAL HAL */
		:*CortexContextSwitch.* 	(.data*) /* Context Switch (PlaformIO build) */
		*libhal.a:          		(.data*) /* Hardware Adaptation Layer */

		*liblibopencm3.a:          	(.data*) /* libopencm3 (CODAL build) */
		*libFrameworkLibOpenCM3.a: 	(.data*) /* libopencm3 (PlatformIO build) */
		*libcocoos.a:				(.data*) /* cocoOS (CODAL build) */
		*libcocoOS_?.?.?.a:			(.data*) /* cocoOS (PlatformIO build) */

		*libgcc.a:          		(.data*) /* C Library */
		*libqfplib.a:          		(.data*) /* Math Library */
		*libnano-float.a:          	(.data*) /* Math Library Replacement */
		*libnewlib.a:          		(.data*) /* C Library Replacement */
		*liblogger.a:          		(.data*) /* Logger */
		*libbluepill.a:          	(.data*) /* Low Level Functions */
		*libbootloader.a:          	(.data*) /* Bootloader */

		. = ALIGN(4);
		_boot_edata = .;
	} >bootram AT >bootrom  /* Stored in bootrom but copied to bootram at startup by reset_handler()  */
	_boot_data_loadaddr = LOADADDR(.boot_data);

	.data : {        		/* Application Data  */
		_data = .;
		*							(.data*) /* Application read-write initialized data */
		. = ALIGN(4);
		_edata = .;
	} >ram AT >rom   		/* Stored in rom but copied to ram at startup by application_start()  */
	_data_loadaddr = LOADADDR(.data);

	.boot_bss : {   		/*  Bootloader Variables */

		/*  Bootloader Variables Init With 0 (BSS)  */
		*libbaseloader.a:          	(.bss*) /* Baseloader */
		:*reset_handler*			(.bss*) /* Reset Handler */
		*libhal.a:*reset_handler*	(.bss*) /* Reset Handler */
		:*codal_target_hal*			(.bss*) /* CODAL HAL */
		*libhal.a:*codal_target_hal*(.bss*) /* CODAL HAL */
		:*CortexContextSwitch*  	(.bss*) /* Context Switch (PlaformIO build) */
		*libhal.a:          		(.bss*) /* Hardware Adaptation Layer */

		*liblibopencm3.a:          	(.bss*) /* libopencm3 (CODAL build) */
		*libFrameworkLibOpenCM3.a: 	(.bss*) /* libopencm3 (PlatformIO build) */
		*libcocoos.a:				(.bss*) /* cocoOS (CODAL build) */
		*libcocoOS_?.?.?.a:			(.bss*) /* cocoOS (PlatformIO build) */

		*libgcc.a:          		(.bss*) /* C Library */
		*libqfplib.a:          		(.bss*) /* Math Library */
		*libnano-float.a:          	(.bss*) /* Math Library Replacement */
		*libnewlib.a:          		(.bss*) /* C Library Replacement */
		*liblogger.a:          		(.bss*) /* Logger */
		*libbluepill.a:          	(.bss*) /* Low Level Functions */
		*libbootloader.a:          	(.bss*) /* Bootloader */

		/*  Bootloader Common Variables  */
		:*reset_handler*			(COMMON) /* Reset Handler */
		*libhal.a:*reset_handler*	(COMMON) /* Reset Handler */
		:*codal_target_hal*			(COMMON) /* CODAL HAL */
		*libhal.a:*codal_target_hal*(COMMON) /* CODAL HAL */
		:*CortexContextSwitch*  	(COMMON) /* Context Switch (PlaformIO build) */
		*libhal.a:          		(COMMON) /* Hardware Adaptation Layer */

		*liblibopencm3.a:          	(COMMON) /* libopencm3 (CODAL build) */
		*libFrameworkLibOpenCM3.a: 	(COMMON) /* libopencm3 (PlatformIO build) */
		*libcocoos.a:				(COMMON) /* cocoOS (CODAL build) */
		*libcocoOS_?.?.?.a:			(COMMON) /* cocoOS (PlatformIO build) */
		
		*libgcc.a:          		(COMMON) /* C Library */
		*libqfplib.a:          		(COMMON) /* Math Library */
		*libnano-float.a:          	(COMMON) /* Math Library Replacement */
		*libnewlib.a:          		(COMMON) /* C Library Replacement */
		*liblogger.a:          		(COMMON) /* Logger */
		*libbluepill.a:          	(COMMON) /* Low Level Functions */
		*libbootloader.a:          	(COMMON) /* Bootloader */

		. = ALIGN(4);
		_boot_ebss = .;

		. = ALIGN(4);
		boot_end = .; /* Previously below */
	} >bootram

	.bss : {       
		/*  Application Variables Init With 0 (BSS)  */
		*							(.bss*)	 /* Application read-write zero initialized data */

		/*  Application Common Variables  */
		*							(COMMON) /* Application common variables */

		. = ALIGN(4);
		_ebss = .;      /* End of BSS section */
	} >ram

	.boot_buf (NOLOAD) : {
		/*  bootbuf contains large buffers used by Bootloader for flashing the ROM.
			In Application Mode, we repurpose boot_buf as stack space.  
			NOLOAD is needed so that arm-none-eabi-objcopy will not output the boot_buf section.
			To check the section attributes: arm-none-eabi-readelf -S .pioenvs/bluepill_f103c8/firmware.elf
		*/
		*                           (.boot_buf*) /* pkt and flashBuf from Bootloader */

		. = ALIGN(4);
		_eboot_buf = .;  /* End of bootbuf region */

		. = ALIGN(4);
		end = .;         /* End of the RAM */
	} >bootbuf

	/*
	 * The .eh_frame section appears to be used for C++ exception handling.
	 * You may need to fix this if you're using C++.
	 */
	/*  Disabled because we don't allow exceptions.
		/DISCARD/ : { *(.eh_frame) }
	*/

}

/* Heap and Stack start after the used part of Application RAM, so we should keep Bootloader RAM small */

PROVIDE(_stack = ORIGIN(bootbuf) + LENGTH(bootbuf));  /*  Application Stack, which includes the bootbuf  */

PROVIDE(_boot_stack = ORIGIN(ram) + LENGTH(ram));     /*  Bootloader Stack, which excludes the bootbuf  */

/* TODO: Check for cross references between Bootloader and Application code/data
NOCROSSREFS_TO(bootrom rom bootdata data bootbuf)
NOCROSSREFS_TO(bootdata bootrom rom data bootbuf)
*/
