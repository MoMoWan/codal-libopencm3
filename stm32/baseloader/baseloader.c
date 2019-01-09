////#define DISABLE_DEBUG
#include <logger/logger.h>
#include <libopencm3/cm3/cortex.h>
#include <libopencm3/cm3/systick.h>  //  For STK_CSR
#include <libopencm3/stm32/desig.h>  //  For DESIG_FLASH_SIZE
#include <libopencm3/stm32/flash.h>
#include <libopencm3/stm32/rcc.h>    //  For RCC_CIR
#include "flash-config.h"
#include "baseloader.h"

#ifdef NOTUSED
//  These must be always at the beginning of the BSS so that different versions of the Baseloader can run.
static uint16_t* dest = NULL;
static uint16_t* src = NULL;
static size_t half_word_count = 0;
static bool verified = false;
static bool should_disable_interrupts = true;
#endif  //  NOTUSED

extern uint32_t _base_etext;  //  End of baseloader code and data, defined in the linker script.
extern void application_start(void);

//  Baseloader Vector Table. Located just after STM32 Vector Table.

__attribute__ ((section(".base_vectors")))
base_vector_table_t base_vector_table = {
	.magic_number   = BASE_MAGIC_NUMBER,  //  Magic number to verify this as a Baseloader Vector Table.
	.version        = BOOTLOADER_VERSION, //  Bootloader version number e.g. 0x 00 01 00 01 for 1.01.
	.baseloader     = baseloader_start,   //  Address of the baseloader function.
	.baseloader_end = &_base_etext,       //  End of the baseloader code and data.
	.application    = application_start,  //  Address of application. Also where the bootloader ends.
};

//  Given an address X, compute the base address of the flash memory page that contains X.
#define FLASH_ADDRESS(x) 		 ( ((uint32_t) x) & ~(FLASH_PAGE_SIZE - 1) )

//  Given an address X, compute the base address of the flash memory page that is >= X (ceiling).
#define FLASH_CEIL_ADDRESS(x)    ( (FLASH_ADDRESS(x) >= (uint32_t) x) ? FLASH_ADDRESS(x) : (FLASH_PAGE_SIZE + FLASH_ADDRESS(x)) )

//  Offset of Base Vector Table from the start of the flash page.
#define BASE_VECTOR_TABLE_OFFSET ( ((uint32_t) &base_vector_table) & (FLASH_PAGE_SIZE - 1) )

//  Given an address X, compute the location of the Base Vector Table of the flash memory page that contains X.
#define BASE_VECTOR_TABLE(x) 	 ( (base_vector_table_t *) ((uint32_t) FLASH_ADDRESS(x) + BASE_VECTOR_TABLE_OFFSET) )

//  Given an address X, is the Base Vector Table in that flash memory page valid (checks magic number)
#define IS_VALID_BASE_VECTOR_TABLE(x)  ( BASE_VECTOR_TABLE(x)->magic_number == BASE_MAGIC_NUMBER )

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

#define base_flash_wait_for_last_operation() { \
	uint32_t flags; \
	base_flash_get_status_flags(flags); \
	while ((flags & FLASH_SR_BSY) == FLASH_SR_BSY) { \
		base_flash_get_status_flags(flags); \
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
	base_flash_wait_for_last_operation(); \
	if ((DESIG_FLASH_SIZE > 512) && (address >= FLASH_BASE+0x00080000)) { \
		FLASH_CR2 |= FLASH_CR_PG; \
	} else { \
		FLASH_CR |= FLASH_CR_PG; \
	} \
	MMIO16(address) = data; \
	base_flash_wait_for_last_operation(); \
	if ((DESIG_FLASH_SIZE > 512) && (address >= FLASH_BASE+0x00080000)) { \
		FLASH_CR2 &= ~FLASH_CR_PG; \
	} else { \
		FLASH_CR &= ~FLASH_CR_PG; \
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
	base_flash_wait_for_last_operation(); \
	if ((DESIG_FLASH_SIZE > 512) \
	    && (page_address >= FLASH_BASE+0x00080000)) { \
		FLASH_CR2 |= FLASH_CR_PER; \
		FLASH_AR2 = page_address; \
		FLASH_CR2 |= FLASH_CR_STRT; \
	} else  { \
		FLASH_CR |= FLASH_CR_PER; \
		FLASH_AR = page_address; \
		FLASH_CR |= FLASH_CR_STRT; \
	} \
	base_flash_wait_for_last_operation(); \
	if ((DESIG_FLASH_SIZE > 512) \
	    && (page_address >= FLASH_BASE+0x00080000)) { \
		FLASH_CR2 &= ~FLASH_CR_PER; \
	} else { \
		FLASH_CR &= ~FLASH_CR_PER; \
	} \
}

//  Disable interrupts for baseloader only because the vector table may be overwritten during flashing.
#define disable_interrupts() { \
	__asm__("CPSID I\n");  /*  Was: cm_disable_interrupts();  */ \
	/* From https://github.com/cnoviello/mastering-stm32 */ \
	STK_CSR = 0;  /* Disables SysTick timer and its related interrupt */ \
	RCC_CIR = 0;  /* Disable all interrupts related to clock */ \
}

//  Get Old Base Vector Table at 0x800 0000.  Get Old Application Address from Old Base Vector Table, truncate to block of 1024 bytes.
//  If Base Magic Number exists at the Old Application Address, then use it as the New Base Vector Table.
//  Get New Application Address from New Base Vector Table.  
//  If Base Magic Number exists at the New Application Address, 
//  then the blocks between Old App Address and New App Address are the new Bootloader Blocks.
//  Flash them to 0x800 0000 and restart.

//  To perform flashing, jump to the New Baseloader Address in the End Base Vector Table,
//  adjusted to the End Base Vector Table Address.

//  This must be the first function in the file.  Macros appearing before the function are OK.
int baseloader_start(uint32_t *dest0, const uint32_t *src0, size_t byte_count) {
	//  TODO: Validate dest, src, half_word_count before flashing.
    //  debug_println("baseloader_start"); debug_force_flush();
	static uint16_t *dest;
	static uint16_t *src;
	static size_t half_word_count;
	static bool verified, should_disable_interrupts;

	dest = (uint16_t *) dest0;
	src =  (uint16_t *) src0;
	half_word_count = byte_count / 2;
	should_disable_interrupts = false;

    static uint16_t *erase_start, *erase_end, *flash_end;
    verified = true; erase_start = NULL; erase_end = NULL;
    flash_end = base_get_flash_end();  //  Remember the bounds of erased data in the current page

	//  Disable interrupts when overwriting the vector table.
	if ((uint32_t) dest == FLASH_BASE) { should_disable_interrupts = true; }
	if (should_disable_interrupts) { disable_interrupts(); } // Only for baseloader.

	base_flash_unlock();  //  TODO: Check MakeCode flashing.
    while (half_word_count > 0) {
        /* Avoid writing past the end of flash */
        if (dest >= flash_end) {  //  debug_println("dest >= flash_end"); debug_flush();
            verified = false;
            break;
        }
        if (dest >= erase_end || dest < erase_start) {
            erase_start = base_get_flash_page_address(dest);
            erase_end = erase_start + (FLASH_PAGE_SIZE)/sizeof(uint16_t);
            base_flash_erase_page((uint32_t)erase_start);
        }
        base_flash_program_half_word((uint32_t)dest, *src);
        erase_start = dest + 1;
        if (*dest != *src) {  //  debug_println("*dest != *src"); debug_flush();
            verified = false;
            break;
        }
        dest++;
        src++;
        half_word_count--;
    }
	base_flash_lock();  //  TODO: Check MakeCode flashing.

	//  Vector table may have been overwritten. Restart to use the new vector table.
	//  TODO: Erase the second vector table.
    //  TODO: if (should_disable_interrupts) { scb_reset_system(); }
	
	return verified ? 1 : 0;
}

int baseloader_fetch(baseloader_func *baseloader_addr, uint32_t **dest, const uint32_t **src, size_t *byte_count) {
	//  Return the address of the baseloader function, located in the Second Base Vector Table.
	//  Also return the parameters to be passed to the baseloader function: dest, src, byte_count.
	if (!baseloader_addr || !dest || !src || !byte_count) { return -1; }
	//  Search for the First and Second Base Vector Tables and find the bootloader range.
	//  First Base Vector Table is in the start of the application ROM.
	if (!IS_VALID_BASE_VECTOR_TABLE(application_start)) { return -2; }  //  Quit if First Base Vector Table is not found.
	base_vector_table_t *begin_base_vector = BASE_VECTOR_TABLE(application_start);

	//  Get size of new bootloader from the First Base Vector Table (same as the Application address).
	uint32_t bootloader_size = (uint32_t) (begin_base_vector->application) - FLASH_BASE;
	if ((uint32_t) application_start + bootloader_size + FLASH_PAGE_SIZE 
		>= FLASH_BASE + FLASH_SIZE_OVERRIDE) { return -3; }  //  Quit if bootloader size is too big.

	//  Second Base Vector Table is at start of application ROM + bootloader size.  Round up to the next flash page.
	uint32_t flash_page_addr = FLASH_CEIL_ADDRESS((uint32_t) application_start + bootloader_size);
	if (!IS_VALID_BASE_VECTOR_TABLE(flash_page_addr)) { return -4; }  //  Quit if Second Base Vector Table is not found.
	base_vector_table_t *end_base_vector = BASE_VECTOR_TABLE(flash_page_addr);

	//  Jump to the baseloader in the Second Base Vector Table.
	*baseloader_addr = (baseloader_func) ((uint32_t) (end_base_vector->baseloader) - FLASH_BASE + flash_page_addr);
	*dest = (uint32_t *) FLASH_BASE;  		 //  Overwrite the old bootloader...
	*src  = (uint32_t *) FLASH_ADDRESS(application_start);  //  By the new bootloader from the Application space.
	*byte_count = bootloader_size;			 //  For this number of bytes.
	return 0;
}

bool base_flash_program_array(uint16_t *dest0, const uint16_t *src0, size_t half_word_count0) {
	//  TODO: Validate dest, src, half_word_count before flashing.
	int status = baseloader_start((uint32_t *) dest0, (const uint32_t *) src0, half_word_count0 * 2);
	return (status == 1);
}

#ifdef DISABLE_DEBUG
#define debug_flash() {}
#define debug_dump() {}
#define debug_dump2() {}
#else
#define debug_flash() { \
    debug_print("target_flash "); debug_printhex_unsigned((size_t) dest); \
    debug_print(", src "); debug_printhex_unsigned((size_t) src);  \
    /* debug_print(" to "); debug_printhex_unsigned((size_t) flash_end); */ \
    debug_print(", hlen "); debug_printhex_unsigned((size_t) half_word_count);  \
    debug_println(should_disable_interrupts ? " DISABLE INTERRUPTS " : " enable interrupts "); \
}
#define debug_dump() { \
    debug_print("src  "); debug_printhex_unsigned((size_t) src); debug_println(""); \
    debug_print("dest "); debug_printhex_unsigned((size_t) dest); debug_println(""); \
    debug_print("size "); debug_printhex_unsigned((size_t) byte_count); debug_println(""); debug_force_flush(); \
    debug_print("before "); debug_printhex_unsigned(*dest); debug_println(""); debug_force_flush(); \
}
#define debug_dump2() { \
    debug_print("after "); debug_printhex_unsigned(*dest); \
    debug_print(" / "); debug_printhex(status); \
	debug_print((*dest == *src) ? " OK " : " FAIL "); \
	debug_println("\r\n"); debug_force_flush(); \
}
#endif  //  DISABLE_DEBUG

void test_copy_bootloader(void) {
	//  Copy bootloader to application space.
	uint32_t bootloader_size = (uint32_t) application_start - FLASH_BASE;
	uint32_t *src =  (uint32_t *) (FLASH_BASE);
	uint32_t *dest = (uint32_t *) FLASH_ADDRESS(application_start);
	size_t byte_count = bootloader_size;
	debug_dump(); ////

	int status = baseloader_start(dest, src, byte_count);
	debug_dump2(); ////

	//  Dump the first base vector.
	base_vector_table_t *begin_base_vector = BASE_VECTOR_TABLE(application_start);
	debug_print("begin_base_vector "); debug_printhex_unsigned((uint32_t) begin_base_vector); debug_println("");
	debug_print("magic "); debug_printhex_unsigned(begin_base_vector->magic_number); debug_println("");
	debug_force_flush();
}

void test_copy_baseloader(void) {
	//  Copy baseloader to end of bootloader.
	uint32_t bootloader_size = (uint32_t) application_start - FLASH_BASE;
	uint32_t baseloader_size = (uint32_t) base_vector_table.baseloader_end - FLASH_BASE;
	uint32_t *src =  (uint32_t *) (FLASH_BASE);
	uint32_t *dest = (uint32_t *) FLASH_CEIL_ADDRESS(application_start + bootloader_size);
	size_t byte_count = baseloader_size;
	debug_dump(); ////

	int status = baseloader_start(dest, src, byte_count);
	debug_dump2(); ////

	//  Dump the second base vector.
	base_vector_table_t *end_base_vector = BASE_VECTOR_TABLE(FLASH_CEIL_ADDRESS(application_start + bootloader_size));
	debug_print("end_base_vector "); debug_printhex_unsigned((uint32_t) end_base_vector); debug_println("");
	debug_print("magic "); debug_printhex_unsigned(end_base_vector->magic_number); debug_println("");
	debug_force_flush();
}

#ifdef NOTUSED
void test_baseloader1(void) {
	//  Test the baseloader: Copy a page from low flash memory to high flash memory.
	test_src =  (uint32_t *) (FLASH_BASE);
	test_dest = (uint32_t *) (FLASH_BASE + FLASH_SIZE_OVERRIDE - FLASH_PAGE_SIZE);
	test_half_word_count = FLASH_PAGE_HALF_WORD_COUNT;
	src = (uint16_t *) test_src; dest = (uint16_t *) test_dest; half_word_count = test_half_word_count; debug_dump(); ////
}

void test_baseloader2(void) {
	//  Test the baseloader: Copy a page from low flash memory to high flash memory.
	test_src =  (uint32_t *) (FLASH_BASE + FLASH_PAGE_SIZE);
	test_dest = (uint32_t *) (FLASH_BASE + FLASH_SIZE_OVERRIDE - FLASH_PAGE_SIZE);
	test_half_word_count = FLASH_PAGE_HALF_WORD_COUNT;
	src = (uint16_t *) test_src; dest = (uint16_t *) test_dest; half_word_count = test_half_word_count; debug_dump(); ////
}

void test_baseloader_end(void) {
	dest = NULL; src = NULL; half_word_count = 0; debug_dump2(); ////
}
#endif  //  NOTUSED
