/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2015 Karl Palsson <karlp@tweak.net.au>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

/* Linker script for STM32F103x8, 64k flash, 20k RAM. */

/* Define memory regions. */
MEMORY
{
	/* ROM is 64K (0x10000). Reserve lower part for bootloader. */
	bootrom (rx)  : ORIGIN = 0x08000000,          LENGTH = 32K
	rom     (rx)  : ORIGIN = 0x08000000 + 0x8000, LENGTH = 32K

	/* RAM is 20K (0x5000). Reserve lower part for bootloader. */
	bootram (rwx) : ORIGIN = 0x20000000,          LENGTH = 10K
	ram     (rwx) : ORIGIN = 0x20000000 + 0x2800, LENGTH = 10K

	/* Heap and Stack start after Firmware RAM, so we should keep Bootloader RAM small */
}

/* Include the common ld script. */
/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2009 Uwe Hermann <uwe@hermann-uwe.de>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

/* Generic linker script for STM32 targets using libopencm3. */

/* Memory regions must be defined in the ld script which includes this one. */

/* Enforce emmition of the vector table. */
EXTERN (vector_table)

/* Define the entry point of the output file. */
ENTRY(reset_handler)

/* Define sections. */
/*
`archive:file' matches file within archive 
`archive:' matches the whole archive 
`:file' matches file but not one in an archive
*/
SECTIONS
{
	/* Bootloader and Firmware ROM */

	.boot_text : {  /* Bootloader Code */
		*(.vectors)	/* Vector table */
		liblibopencm3.a:*(.text*)	/* Program code */
		libFrameworkLibOpenCM3.a:*(.text*)	/* Program code */
		. = ALIGN(4);
		*(.rodata*)	/* Read-only data */
		. = ALIGN(4);
	} >bootrom

	.text : {       /* Firmware Code */
		*(.vectors)	/* Vector table */
		*(.text*)	/* Program code */
		. = ALIGN(4);
		*(.rodata*)	/* Read-only data */
		. = ALIGN(4);
	} >rom

	.boot_preinit_array : { /* Bootloader Constructors */
		. = ALIGN(4);
		__boot_preinit_array_start = .;
		KEEP (*(.preinit_array))
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
		KEEP (*(SORT_BY_NAME (.init_array.*)))  /* Sort by name */
		KEEP (*(.init_array))
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

	/* Not needed because we don't call C++ destructors.
		.fini_array : {
			. = ALIGN(4);
			__fini_array_start = .;
			KEEP (*(.fini_array))
			KEEP (*(SORT(.fini_array.*)))
			__fini_array_end = .;
		} >rom
	*/

	/*
	 * Another section used by C++ stuff, appears when using newlib with
	 * 64bit (long long) printf support
	 */
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

	.boot_data : {          /* Bootloader Data  */
		_boot_data = .;
		*(.data*)	        /* Read-write initialized data */
		. = ALIGN(4);
		_boot_edata = .;
	} >bootram AT >bootrom  /* Stored in bootrom but copied to bootram at startup  */
	_boot_data_loadaddr = LOADADDR(.boot_data);

	.data : {        /* Firmware Data  */
		_data = .;
		*(.data*)	 /* Read-write initialized data */
		. = ALIGN(4);
		_edata = .;
	} >ram AT >rom   /* Stored in rom but copied to ram at startup  */
	_data_loadaddr = LOADADDR(.data);

	.boot_bss : {   /*  Bootloader Variables */
		*(.bss*)	/*  Read-write zero initialized data */
		*(COMMON)
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
	/*  TODO: Disabled
		/DISCARD/ : { *(.eh_frame) }
	*/

}

/* Heap and Stack start after Firmware RAM, so we should keep Bootloader RAM small */

PROVIDE(_stack = ORIGIN(ram) + LENGTH(ram));
