/* Linker script for STM32 Blue Pill Bootloader + Application: STM32F103x8, 64k flash, 20k RAM. 
Bootloader resides in low memory (always accessible) and consists of these modules:
libopencm3
cocoos
qfplib
nano-float
newlib
logger
bluepill
bootloader
hal (except unit test)

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
unit test */

/* Define memory regions. */
MEMORY
{
	/* Available ROM is 64K (0x10000). Reserve lower part for bootloader, upper part for Application. */
	bootrom (rx)  : ORIGIN = 0x08000000,          LENGTH = 24K
	rom     (rx)  : ORIGIN = 0x08000000 + 0x6000, LENGTH = 40K

	/* Available RAM is 20K (0x5000). Reserve lower part for bootloader, upper part for Application. */
	bootram (rwx) : ORIGIN = 0x20000000,          LENGTH = 10K
	ram     (rwx) : ORIGIN = 0x20000000 + 0x2800, LENGTH = 10K

	/* Heap and Stack start after Application RAM, so we should keep Bootloader RAM small */
}

/* Force vector table to be linked in. */
EXTERN(vector_table)

/* Execution starts at the wrapped reset_handler() function in stm32/hal/reset_handler.c. */
ENTRY(reset_handler)

/* Define sections in the executable file, by selecting input files (.o) and archives (.a)
	`archive:file' matches file within archive 
	`archive:' matches the whole archive 
	`:file' matches file but not one in an archive */
SECTIONS
{
	/* Bootloader and Application ROM */

	.boot_text : {  /* Bootloader ROM */
		_boot_text = .;				/* We will use _boot_text to determine the start of the bootloader. */
		*                           (.vectors) /* Vector table */

		/* Bootloader Code */
		:*reset_handler*			(.text*) /* Reset Handler (CODAL build) */
		*libhal.a:*reset_handler*	(.text*) /* Reset Handler */
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
		:*reset_handler*			(.rodata*) /* Reset Handler */
		*libhal.a:*reset_handler*	(.rodata*) /* Reset Handler */
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

	.text : {       /* Application ROM */
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
		/* No constructors for our low-level bootloader functions, otherwise it may cause a loop */
		__boot_preinit_array_end = .;
	} >bootrom

	.preinit_array : { /* Application Constructors */
		. = ALIGN(4);
		__preinit_array_start = .;
		KEEP (*						(.preinit_array)) /* Application Constructors */
		__preinit_array_end = .;
	} >rom

	.boot_init_array : { /* Bootloader Constructors */
		. = ALIGN(4);
		__boot_init_array_start = .; 
		/* No constructors for our low-level bootloader functions, otherwise it may cause a loop */
		__boot_init_array_end = .;
		. = ALIGN(4);  /* Previously below */
		_boot_etext = .;
	} >bootrom

	.init_array : { /* Application Constructors */
		. = ALIGN(4);
		__init_array_start = .;
		KEEP (*					(SORT_BY_NAME (.init_array.*))) /* Application Constructors. */
		KEEP (*					(.init_array)) 					/* Application Constructors. */
		__init_array_end = .;

		. = ALIGN(4);  /* Previously below */
		_etext = .;
	} >rom

	/* Not needed because we don't call C++ destructors. We provide a dummy __fini_array_start, __fini_array_end to satisfy libopencm3/vector.c */
	.fini_array : {
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
	} >bootram AT >bootrom  /* Stored in bootrom but copied to bootram at startup  */
	_boot_data_loadaddr = LOADADDR(.boot_data);

	.data : {        /* Application Data  */
		_data = .;
		*							(.data*) /* Application read-write initialized data */
		. = ALIGN(4);
		_edata = .;
	} >ram AT >rom   /* Stored in rom but copied to ram at startup  */
	_data_loadaddr = LOADADDR(.data);

	.boot_bss : {   /*  Bootloader Variables */

		/*  Bootloader Variables Init With 0 (BSS)  */
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

/* Heap and Stack start after Application RAM, so we should keep Bootloader RAM small */

PROVIDE(_stack = ORIGIN(ram) + LENGTH(ram));
