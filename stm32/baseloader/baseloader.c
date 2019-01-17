////#define DISABLE_DEBUG
#include <logger/logger.h>
#include <libopencm3/cm3/cortex.h>
#include <libopencm3/cm3/systick.h>  //  For STK_CSR
#include <libopencm3/cm3/scb.h>
#include <libopencm3/cm3/vector.h>   //  For vector_table_t  
#include <libopencm3/stm32/desig.h>  //  For DESIG_FLASH_SIZE
#include <libopencm3/stm32/flash.h>
#include <libopencm3/stm32/rcc.h>    //  For RCC_CIR
#include "flash-config.h"
#include "baseloader.h"

extern void application_start(void);
extern vector_table_t vector_table;

//  Baseloader Vector Table. Located just after STM32 Vector Table.
__attribute__ ((section(".base_vectors")))
base_vector_table_t base_vector_table = {
	.magic_number   = BASE_MAGIC_NUMBER,  //  Magic number to verify this as a Baseloader Vector Table.
	.version        = BOOTLOADER_VERSION, //  Bootloader version number e.g. 0x 00 01 00 01 for 1.01.
	.baseloader     = baseloader_start,   //  Address of the baseloader function.
	.baseloader_end = &_base_etext,       //  End of the baseloader code and data.
	.application    = application_start,  //  Address of application. Also where the bootloader ends.
	.magic_number2  = BASE_MAGIC_NUMBER2, //  Second magic number to verify that the Baseloader Vector Table was not truncated.
};

//  To support different calling conventions for Baseloader we don't allow stack parameters.  All parameters must be passed via base_para at a fixed address (start of RAM).
__attribute__ ((section(".base_para")))
base_para_t base_para = {
	.dest = 0,
	.src = 0,
	.byte_count = 0,
	.restart = 0,
	.preview = 0,
	.result = 0,
	.fail = 0,
};

//  Temporary variables placed in base_tmp at a fixed address.
typedef struct {
	uint16_t *dest_hw, *src_hw;
	size_t half_word_count;
	int bytes_flashed, verified;
    uint16_t *erase_start, *erase_end, *flash_end;
	volatile uint32_t flags;
} __attribute__((packed)) base_tmp_t;

__attribute__ ((section(".base_tmp")))
base_tmp_t base_tmp = {
	.dest_hw = NULL,
	.src_hw = NULL,
	.half_word_count = 0,
	.bytes_flashed = 0,
	.verified = 0,
    .erase_start = NULL,
	.erase_end = NULL,
	.flash_end = NULL,
	.flags = 0,
};

//  We will set a watchpoint so that the debugger will stop when the bootloader has been flashed.
int debug_base_result;

//  Timeout when waiting for flash operation.
#define base_timeout (0x100ul)

#ifdef FLASH_SIZE_OVERRIDE
    /* Allow access to the flash size we define */
    #define base_get_flash_end() ((uint16_t*)(FLASH_BASE + FLASH_SIZE_OVERRIDE))
#else
    /* Only allow access to the chip's self-reported flash size */
    #define base_get_flash_end() ((uint16_t*)(FLASH_BASE + (size_t)DESIG_FLASH_SIZE*FLASH_PAGE_SIZE))
#endif

//  Returns uint16_t*
#define base_get_flash_page_address(/* uint16_t* */ dest) ( \
    (uint16_t*)(((uint32_t)dest / FLASH_PAGE_SIZE) * FLASH_PAGE_SIZE) \
)

//  Flash functions for Baseloader, prefixed by "base_flash". We define them here instead of using libopencm3 to prevent any external references.
//  We use macros to avoid absolute address references to functions, since the Baseloader must run in low and high memory.
//  TODO: Support other than F1

//  Based on https://github.com/libopencm3/libopencm3/blob/master/lib/stm32/common/flash_common_f.c

/* Authorize the FPEC access. */
#define base_flash_unlock() { \
	FLASH_KEYR = FLASH_KEYR_KEY1; \
	FLASH_KEYR = FLASH_KEYR_KEY2; \
}

#define base_flash_lock() { \
	FLASH_CR |= FLASH_CR_LOCK; \
}

//  Based on https://github.com/libopencm3/libopencm3/blob/master/lib/stm32/f1/flash.c

/*---------------------------------------------------------------------------*/
/** @brief Read All Status Flags
The programming error, end of operation, write protect error and busy flags
are returned in the order of appearance in the status register.
Flags for the upper bank, where appropriate, are combined with those for
the lower bank using bitwise OR, without distinction.
@returns uint32_t. bit 0: busy, bit 2: programming error, bit 4: write protect
error, bit 5: end of operation.
*/

#define base_flash_get_status_flags(flags) { \
	flags = (FLASH_SR & (FLASH_SR_PGERR | \
			FLASH_SR_EOP | \
			FLASH_SR_WRPRTERR | \
			FLASH_SR_BSY)); \
	if (DESIG_FLASH_SIZE > 512) { \
		flags |= (FLASH_SR2 & (FLASH_SR_PGERR | \
			FLASH_SR_EOP | \
			FLASH_SR_WRPRTERR | \
			FLASH_SR_BSY)); \
	} \
}

//  Based on https://github.com/libopencm3/libopencm3/blob/master/lib/stm32/common/flash_common_f01.c

#define base_flash_wait_for_last_operation(error_result) { \
	base_para.result = 0; \
	base_para.fail = 0; \
	base_tmp.flags = 0; \
	base_flash_get_status_flags(base_tmp.flags); \
	while ((base_tmp.flags & FLASH_SR_BSY) == FLASH_SR_BSY) { \
		if (base_para.fail++ >= base_timeout) { base_para.result = error_result; break; } \
		base_tmp.flags = 0; \
		base_flash_get_status_flags(base_tmp.flags); \
	} \
}

/*---------------------------------------------------------------------------*/
/** @brief Program a Half Word to FLASH
This performs all operations necessary to program a 16 bit word to FLASH memory.
The program error flag should be checked separately for the event that memory
was not properly erased.
Status bit polling is used to detect end of operation.
@param[in] address Full address of flash half word to be programmed.
@param[in] data half word to write
*/

#define base_flash_program_half_word(/* uint32_t */ address, /* uint16_t */ data) { \
	base_flash_wait_for_last_operation(-10); \
	if (base_para.result == 0) { \
		if ((DESIG_FLASH_SIZE > 512) && (address >= (FLASH_BASE+0x00080000))) { \
			FLASH_CR2 |= FLASH_CR_PG; \
		} else { \
			FLASH_CR |= FLASH_CR_PG; \
		} \
		MMIO16(address) = data; \
		base_flash_wait_for_last_operation(-11); \
		if (base_para.result == 0) { \
			if ((DESIG_FLASH_SIZE > 512) && (address >= (FLASH_BASE+0x00080000))) { \
				FLASH_CR2 &= ~FLASH_CR_PG; \
			} else { \
				FLASH_CR &= ~FLASH_CR_PG; \
			} \
		} \
	} \
}

/*---------------------------------------------------------------------------*/
/** @brief Erase a Page of FLASH
This performs all operations necessary to erase a page in FLASH memory.
The page should be checked to ensure that it was properly erased. A page must
first be fully erased before attempting to program it.
Note that the page sizes differ between devices. See the reference manual or
the FLASH programming manual for details.
@param[in] page_address Full address of flash page to be erased.
*/

#define base_flash_erase_page(/* uint32_t */ page_address) { \
	base_flash_wait_for_last_operation(-12); \
	if (base_para.result == 0) { \
		if ((DESIG_FLASH_SIZE > 512) \
			&& (page_address >= (FLASH_BASE+0x00080000))) { \
			FLASH_CR2 |= FLASH_CR_PER; \
			FLASH_AR2 = page_address; \
			FLASH_CR2 |= FLASH_CR_STRT; \
		} else  { \
			FLASH_CR |= FLASH_CR_PER; \
			FLASH_AR = page_address; \
			FLASH_CR |= FLASH_CR_STRT; \
		} \
		base_flash_wait_for_last_operation(-13); \
		if (base_para.result == 0) { \
			if ((DESIG_FLASH_SIZE > 512) \
				&& (page_address >= (FLASH_BASE+0x00080000))) { \
				FLASH_CR2 &= ~FLASH_CR_PER; \
			} else { \
				FLASH_CR &= ~FLASH_CR_PER; \
			} \
		} \
	} \
}

//  Disable interrupts while flashing Bootloader because the System Vector Table may have been overwritten during flashing.
#define base_disable_interrupts() { \
	__asm__("CPSID I\n");  /*  Was: cm_disable_interrupts();  */ \
	/* From https://github.com/cnoviello/mastering-stm32 */ \
	STK_CSR = 0;  /* Disables SysTick timer and its related interrupt */ \
	RCC_CIR = 0;  /* Disable all interrupts related to clock */ \
}

//  Restart the device after flashing Bootloader because the System Vector Table may have been overwritten during flashing.  From scb_reset_system()
#define base_scb_reset_system() { \
	SCB_AIRCR = SCB_AIRCR_VECTKEY | SCB_AIRCR_SYSRESETREQ; \
	for (;;) {}  \
}

//  Get Old Base Vector Table at 0x800 0000.  Get Old Application Address from Old Base Vector Table, truncate to block of 1024 bytes.
//  If Base Magic Number exists at the Old Application Address, then use it as the New Base Vector Table.
//  Get New Application Address from New Base Vector Table.  
//  If Base Magic Number exists at the New Application Address, 
//  then the blocks between Old App Address and New App Address are the new Bootloader Blocks.
//  Flash them to 0x800 0000 and restart.

//  To perform flashing, jump to the New Baseloader Address in the End Base Vector Table,
//  adjusted to the End Base Vector Table Address.

/* Warning: PlatformIO and CODAL Builds use different stack conventions. So we avoid using the stack for Baseloader.
	PlatformIO Build:
	08000168 <baseloader_start>:
		static uint16_t *src;
		static size_t half_word_count;
		static int bytes_flashed;
		static bool verified, restart;

		dest = (uint16_t *) dest0;
	8000168:	4b8b      	ldr	r3, [pc, #556]	; (8000398 <baseloader_start+0x230>)
	800016a:	6018      	str	r0, [r3, #0]
		src =  (uint16_t *) src0;
	800016c:	4b8b      	ldr	r3, [pc, #556]	; (800039c <baseloader_start+0x234>)
	800016e:	6019      	str	r1, [r3, #0]

	CODAL Build: 
	int baseloader_start(uint32_t *dest0, const uint32_t *src0, size_t byte_count) {
	8000168:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
		half_word_count = byte_count / 2;
		bytes_flashed = 0;
		restart = false;

		static uint16_t *erase_start, *erase_end, *flash_end;
		verified = true; erase_start = NULL; erase_end = NULL;
	800016c:	2501      	movs	r5, #1
		bytes_flashed = 0;
	800016e:	2300      	movs	r3, #0
		half_word_count = byte_count / 2;
	8000170:	fa22 fc05 	lsr.w	ip, r2, r5
		dest = (uint16_t *) dest0;
	8000174:	4aa9      	ldr	r2, [pc, #676]	; (800041c <baseloader_start+0x2b4>)  */

//  This must be the first function in the file.  Macros appearing before the function are OK.
void baseloader_start(void) {
	//  Copy the Bootloader from src to dest for byte_count bytes.  Return the number of bytes flashed in result.  
	//  To support different calling conventions for Baseloader we don't allow stack parameters.  All parameters must be passed via base_para at a fixed address.
	//  uint32_t dest;  		//  Destination address for new Bootloader.
	//  uint32_t src;  			//  Source address for new Bootloader.
	//  uint32_t byte_count;	//  Byte size of new Bootloader.
	//  uint32_t restart;  		//  Set to 1 if we should restart the device after copying Bootloader.
	//  int result;				//  Number of bytes copied, or negative for error.
	//  uint32_t fail;  		//  Address that caused the Baseloader to fail.

	base_tmp.dest_hw = (uint16_t *) base_para.dest;
	base_tmp.src_hw  = (uint16_t *) base_para.src;
	base_tmp.half_word_count = base_para.byte_count / 2;
	base_para.result = 0;
	base_para.fail = 0;
	base_tmp.bytes_flashed = 0;
    base_tmp.verified = true; base_tmp.erase_start = NULL; base_tmp.erase_end = NULL;
    base_tmp.flash_end = base_get_flash_end();  //  Remember the bounds of erased data in the current page

	//  Validate dest, src, byte_count before flashing.
    //  TODO: Support other memory sizes.
	if ((uint32_t) base_para.dest < 0x08000000) {  //  Dest ROM address too low.
		base_para.result = -2;
		base_para.fail = (uint32_t) base_para.dest;
		return;
	}
	if (((uint32_t) base_para.dest) + base_para.byte_count > 0x08010000) {  //  Dest ROM address too high.
		base_para.result = -3;
		base_para.fail = ((uint32_t) base_para.dest) + base_para.byte_count;
		return;
	}
	if ((uint32_t) base_para.src >= 0x08000000 &&
		(uint32_t) base_para.src < 0x08010000) {  //  If src is in ROM...

		if (((uint32_t) base_para.src) + base_para.byte_count > 0x08010000) {  //  Too many ROM bytes to copy.
			base_para.result = -4;
			base_para.fail = ((uint32_t) base_para.src) + base_para.byte_count;
			return;
		}
	} else if ((uint32_t) base_para.src >= 0x20000000 &&
		(uint32_t) base_para.src < 0x20005000) {  //  If src is in RAM...

		if (((uint32_t) base_para.src) + base_para.byte_count > 0x20005000) {  //  Too many RAM bytes to copy.
			base_para.result = -5;
			base_para.fail = ((uint32_t) base_para.src) + base_para.byte_count;
			return;
		}
	} else {  //  Src is in neither RAM or ROM.
		base_para.result = -6;
		base_para.fail = (uint32_t) base_para.src;
		return;
	}

	//  Disable interrupts while flashing Bootloader because the System Vector Table may have been overwritten during flashing.
	if (base_para.restart) { base_disable_interrupts(); }

	base_flash_unlock();  if (base_para.result < 0) { return; }  //  Quit if error.
    while (base_tmp.half_word_count > 0) {        
		if (!base_para.preview && base_para.restart) { 
			if (base_tmp.bytes_flashed % 100 == 0) {
				debug_base_result = base_tmp.bytes_flashed;  //  Trigger a watchpoint break.
			}
		}
        if (base_tmp.dest_hw >= base_tmp.flash_end) {  /* Avoid writing past the end of flash */
            base_tmp.verified = false;
            break;
        }
        if (base_tmp.dest_hw >= base_tmp.erase_end || base_tmp.dest_hw < base_tmp.erase_start) {
            base_tmp.erase_start = base_get_flash_page_address(base_tmp.dest_hw);
            base_tmp.erase_end = base_tmp.erase_start + ((FLASH_PAGE_SIZE) / sizeof(uint16_t));
			if (!base_para.preview) {  //  Erase the ROM page.
            	base_flash_erase_page((uint32_t) base_tmp.erase_start);
				if (base_para.result < 0) { return; }  //  Quit if error.
			}
        }
		if (!base_para.preview) {  //  Write the ROM half-word.
        	base_flash_program_half_word((uint32_t) base_tmp.dest_hw, *base_tmp.src_hw);
			if (base_para.result < 0) { return; }  //  Quit if error.
		}
        base_tmp.erase_start = base_tmp.dest_hw + 1;
        if (!base_para.preview && *base_tmp.dest_hw != *base_tmp.src_hw) {
            base_tmp.verified = false;
            break;
        }
        base_tmp.dest_hw++;
        base_tmp.src_hw++;
        base_tmp.half_word_count--;
		base_tmp.bytes_flashed += 2;
    }
	base_flash_lock();  if (base_para.result < 0) { return; }  //  Quit if error.

	//  TODO: Erase the second vector table.

	base_para.result = base_tmp.verified ? base_tmp.bytes_flashed : -1;  //  Returns -1 if verification failed.

	//  Restart the device after flashing Bootloader because the System Vector Table may have been overwritten during flashing.
    if (!base_para.preview && base_para.restart) { 
		//  Swap back to the original System Vector Table.  DMB and DSB may not be necessary for some processors.
        //  See http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dai0321a/BIHHDGBC.html
        asm("dmb");
		SCB_VTOR = (uint32_t) &vector_table;  
        asm("dsb");
		debug_base_result = base_para.result;  //  Trigger a watchpoint break after flashing.
		base_scb_reset_system();  //  Restart.
	}	
}

int baseloader_fetch(baseloader_func *baseloader_addr, uint32_t **dest, const uint32_t **src, size_t *byte_count) {
	//  Return the address of the baseloader function, located in the Second Base Vector Table.
	//  Also return the parameters to be passed to the baseloader function: dest, src, byte_count.
	if (!baseloader_addr || !dest || !src || !byte_count) { 
		base_para.fail = (uint32_t) baseloader_addr;
		return -1; 
	}
	//  Search for the First and Second Base Vector Tables and find the bootloader range.
	//  First Base Vector Table is in the start of the application ROM.
	if (!IS_VALID_BASE_VECTOR_TABLE(application_start)) {  //  Quit if First Base Vector Table is not found.
		base_para.fail = (uint32_t) FLASH_ADDRESS(application_start);
		return -2; 
	}
	base_vector_table_t *begin_base_vector = BASE_VECTOR_TABLE(application_start);

	//  Get size of new bootloader from the First Base Vector Table (same as the Application ROM start address).
	uint32_t bootloader_size = (uint32_t) FLASH_ADDRESS(begin_base_vector->application) - FLASH_BASE;
	if ((uint32_t) application_start + bootloader_size + FLASH_PAGE_SIZE 
		>= FLASH_BASE + FLASH_SIZE_OVERRIDE) { //  Quit if bootloader size is too big.
		base_para.fail = bootloader_size;
		return -3; 
	} 

	//  Second Base Vector Table is at start of application ROM + bootloader size.  TODO: Round up to the next flash page?
	uint32_t flash_page_addr = (uint32_t) FLASH_ADDRESS(application_start) + bootloader_size;
	if (!IS_VALID_BASE_VECTOR_TABLE(flash_page_addr)) {  //  Quit if Second Base Vector Table is not found.
		base_para.fail = flash_page_addr;
		*byte_count = bootloader_size;
		return -4;
	}
	base_vector_table_t *end_base_vector = BASE_VECTOR_TABLE(flash_page_addr);

	//  Tell caller to jump to the baseloader in the Second Base Vector Table.
	*baseloader_addr = (baseloader_func) ((uint32_t) (end_base_vector->baseloader) - FLASH_BASE + flash_page_addr);
	*dest = (uint32_t *) FLASH_BASE;  		 //  Overwrite the old bootloader...
	*src  = (uint32_t *) FLASH_ADDRESS(application_start);  //  By the new bootloader from the Application space.
	*byte_count = bootloader_size;			 //  For this number of bytes.
	return 0;
}
