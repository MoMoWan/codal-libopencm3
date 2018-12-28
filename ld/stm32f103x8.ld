/* Linker script for STM32 Blue Pill Bootloader + Firmware: STM32F103x8, 64k flash, 20k RAM. 
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

Firmware resides in high memory and consists of these modules:
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
	/* Available ROM is 64K (0x10000). Reserve lower part for bootloader, upper part for firmware. */
	bootrom (rx)  : ORIGIN = 0x08000000,          LENGTH = 24K
	rom     (rx)  : ORIGIN = 0x08000000 + 0x6000, LENGTH = 40K

	/* Available RAM is 20K (0x5000). Reserve lower part for bootloader, upper part for firmware. */
	bootram (rwx) : ORIGIN = 0x20000000,          LENGTH = 10K
	ram     (rwx) : ORIGIN = 0x20000000 + 0x2800, LENGTH = 10K

	/* Heap and Stack start after Firmware RAM, so we should keep Bootloader RAM small */
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
	/DISCARD/ : { 
		*zzzliblibopencm3.a:          	(.text.reset_handler) /* libopencm3 (CODAL build) */
		*zzzlibFrameworkLibOpenCM3.a: 	(.text.reset_handler) /* libopencm3 (PlatformIO build) */
	}

	/* Bootloader and Firmware ROM */

	.boot_text : {  /* Bootloader ROM */
		*                           (.vectors) /* Vector table */

		/* Bootloader Code */
		*libhal.a:*reset_handler*	(.text*) /* Reset Handler */
		*libhal.a:*codal_target_hal*(.text*) /* CODAL HAL */
		*libhal.a:          		(.text*) /* Hardware Adaptation Layer */
		:*CortexContextSwitch*	    (.text*) /* Context Switch (PlaformIO build) */

     	*liblibopencm3.a:(EXCLUDE_FILE (*vector*.o) .text*)
     	*libFrameworkLibOpenCM3.a:(EXCLUDE_FILE (*vector*.o) .text*)

		*zzzliblibopencm3.a:          	(.text*) /* libopencm3 (CODAL build) */
		*zzzlibFrameworkLibOpenCM3.a: 	(.text*) /* libopencm3 (PlatformIO build) */
		*libcocoos.a:				(.text*) /* cocoOS (CODAL build) */
		*libcocoOS_?.?.?.a:			(.text*) /* cocoOS (PlatformIO build) */

		*libqfplib.a:          		(.text*) /* Math Library */
		*libnano-float.a:          	(.text*) /* Math Library Replacement */
		*libnewlib.a:          		(.text*) /* C Library Replacement */
		*liblogger.a:          		(.text*) /* Logger */
		*libbluepill.a:          	(.text*) /* Low Level Functions */
		*libbootloader.a:          	(.text*) /* Bootloader */

		. = ALIGN(4);  /* Bootloader Read-only data e.g. literal strings */
		*libhal.a:*reset_handler*	(.rodata*) /* Reset Handler */
		*libhal.a:*codal_target_hal*(.rodata*) /* CODAL HAL */
		*libhal.a:          		(.rodata*) /* Hardware Adaptation Layer */
		:*CortexContextSwitch*	    (.rodata*) /* Context Switch (PlaformIO build) */

		*liblibopencm3.a:          	(.rodata*) /* libopencm3 (CODAL build) */
		*libFrameworkLibOpenCM3.a: 	(.rodata*) /* libopencm3 (PlatformIO build) */
		*libcocoos.a:				(.rodata*) /* cocoOS (CODAL build) */
		*libcocoOS_?.?.?.a:			(.rodata*) /* cocoOS (PlatformIO build) */

		*libqfplib.a:          		(.rodata*) /* Math Library */
		*libnano-float.a:          	(.rodata*) /* Math Library Replacement */
		*libnewlib.a:          		(.rodata*) /* C Library Replacement */
		*liblogger.a:          		(.rodata*) /* Logger */
		*libbluepill.a:          	(.rodata*) /* Low Level Functions */
		*libbootloader.a:          	(.rodata*) /* Bootloader */

		. = ALIGN(4);
	} >bootrom

	.text : {       /* Firmware ROM */
		*                           (.vectors) /* Vector table */
		*libunity.a:          		(.text*)   /* Unit Test */
		:*unit_test.o				(.text*)   /* Unit Test (if any). Moved here to save space. Can only be called by firmware, not bootloader. */
		*							(.text*)   /* Other Firmware Code */

		. = ALIGN(4);
		*libunity.a:          		(.rodata*) /* Unit Test */
		:*unit_test.o			    (.rodata*) /* Unit Test (if any) */
		*							(.rodata*) /* Other Firmware Read-only data */
		. = ALIGN(4);
	} >rom

	.boot_preinit_array : { /* Bootloader Constructors */
		. = ALIGN(4);
		__boot_preinit_array_start = .;  /*  We use KEEP to force these symbols to be linked, otherwise they will be removed because they are not referenced */
		/* No constructors for our low-level bootloader functions, otherwise it may cause a loop */
		__boot_preinit_array_end = .;
	} >bootrom

	.preinit_array : { /* Firmware Constructors */
		. = ALIGN(4);
		__preinit_array_start = .;
		KEEP (*libunity.a:			(.preinit_array)) /* Unit Test Constructors (if any) */
		KEEP (:*unit_test.o			(.preinit_array)) /* Unit Test Constructors (if any) */
		KEEP (*						(.preinit_array)) /* Other Firmware Constructors */
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

	.init_array : { /* Firmware Constructors */
		. = ALIGN(4);
		__init_array_start = .;
		KEEP (*libunity.a:		(SORT_BY_NAME (.init_array.*))) /* Unit Test Constructors (if any) */
		KEEP (:*unit_test.o		(SORT_BY_NAME (.init_array.*))) /* Unit Test Constructors (if any) */
		KEEP (*					(SORT_BY_NAME (.init_array.*))) /* Other Firmware Constructors. */

		KEEP (*libunity.a:		(.init_array)) /* Unit Test Constructors (if any) */
		KEEP (:*unit_test.o		(.init_array)) /* Unit Test Constructors (if any) */
		KEEP (*					(.init_array)) /* Other Firmware Constructors. */
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

	.boot_data : {          /* Bootloader RAM */
		_boot_data = .;

		/* Bootloader read-write initialized data */
		*libhal.a:*reset_handler*	(.data*) /* Reset Handler */
		*libhal.a:*codal_target_hal*(.data*) /* CODAL HAL */
		*libhal.a:          		(.data*) /* Hardware Adaptation Layer */
		:*CortexContextSwitch.* 	(.data*) /* Context Switch (PlaformIO build) */

		*liblibopencm3.a:          	(.data*) /* libopencm3 (CODAL build) */
		*libFrameworkLibOpenCM3.a: 	(.data*) /* libopencm3 (PlatformIO build) */
		*libcocoos.a:				(.data*) /* cocoOS (CODAL build) */
		*libcocoOS_?.?.?.a:			(.data*) /* cocoOS (PlatformIO build) */

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

	.data : {        /* Firmware Data  */
		_data = .;
		*libunity.a:          		(.data*) /* Unit Test */
		:*unit_test.o			    (.data*) /* Unit Test (if any) */
		*							(.data*) /* Other Firmware read-write initialized data */
		. = ALIGN(4);
		_edata = .;
	} >ram AT >rom   /* Stored in rom but copied to ram at startup  */
	_data_loadaddr = LOADADDR(.data);

	.boot_bss : {   /*  Bootloader Variables */

		/*  Bootloader Variables Init With 0 (BSS)  */
		*libhal.a:*reset_handler*	(.bss*) /* Reset Handler */
		*libhal.a:*codal_target_hal*(.bss*) /* CODAL HAL */
		*libhal.a:          		(.bss*) /* Hardware Adaptation Layer */
		:*CortexContextSwitch*  	(.bss*) /* Context Switch (PlaformIO build) */

		*liblibopencm3.a:          	(.bss*) /* libopencm3 (CODAL build) */
		*libFrameworkLibOpenCM3.a: 	(.bss*) /* libopencm3 (PlatformIO build) */
		*libcocoos.a:				(.bss*) /* cocoOS (CODAL build) */
		*libcocoOS_?.?.?.a:			(.bss*) /* cocoOS (PlatformIO build) */

		*libqfplib.a:          		(.bss*) /* Math Library */
		*libnano-float.a:          	(.bss*) /* Math Library Replacement */
		*libnewlib.a:          		(.bss*) /* C Library Replacement */
		*liblogger.a:          		(.bss*) /* Logger */
		*libbluepill.a:          	(.bss*) /* Low Level Functions */
		*libbootloader.a:          	(.bss*) /* Bootloader */

		/*  Bootloader Common Variables  */
		*libhal.a:*reset_handler*	(COMMON) /* Reset Handler */
		*libhal.a:*codal_target_hal*(COMMON) /* CODAL HAL */
		*libhal.a:          		(COMMON) /* Hardware Adaptation Layer */
		:*CortexContextSwitch*  	(COMMON) /* Context Switch (PlaformIO build) */

		*liblibopencm3.a:          	(COMMON) /* libopencm3 (CODAL build) */
		*libFrameworkLibOpenCM3.a: 	(COMMON) /* libopencm3 (PlatformIO build) */
		*libcocoos.a:				(COMMON) /* cocoOS (CODAL build) */
		*libcocoOS_?.?.?.a:			(COMMON) /* cocoOS (PlatformIO build) */
		
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
		/*  Firmware Variables Init With 0 (BSS)  */
		*libunity.a:          		(.bss*) /* Unit Test */
		:*unit_test.o			    (.bss*)  /* Unit Test zero initialized data (if any) */
		*							(.bss*)	 /* Other Firmware read-write zero initialized data */

		/*  Firmware Common Variables  */
		*libunity.a:          		(COMMON) /* Unit Test */
		:*unit_test.o			    (COMMON) /* Unit Test (if any) */
		*							(COMMON) /* Other Firmware common variables */

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
