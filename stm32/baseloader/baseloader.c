#include <logger/logger.h>
#include <libopencm3/cm3/cortex.h>
#include <libopencm3/stm32/desig.h>  //  For DESIG_FLASH_SIZE
#include <libopencm3/stm32/flash.h>
#include "flash-config.h"
#include "baseloader.h"

//  Flash functions for Baseloader, prefixed by "base_flash". We define them here instead of using libopencm3 to prevent any external references.
//  We use macros to avoid absolute address references to functions, since the Baseloader must run in low and high memory.
//  TODO: Support other than F1

//  Based on https://github.com/libopencm3/libopencm3/blob/master/lib/stm32/common/flash_common_f.c

void base_flash_unlock(void)
{
	/* Authorize the FPEC access. */
	FLASH_KEYR = FLASH_KEYR_KEY1;
	FLASH_KEYR = FLASH_KEYR_KEY2;
}

void base_flash_lock(void)
{
	FLASH_CR |= FLASH_CR_LOCK;
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

uint32_t base_flash_get_status_flags(void)
{
	uint32_t flags = (FLASH_SR & (FLASH_SR_PGERR |
			FLASH_SR_EOP |
			FLASH_SR_WRPRTERR |
			FLASH_SR_BSY));
	if (DESIG_FLASH_SIZE > 512) {
		flags |= (FLASH_SR2 & (FLASH_SR_PGERR |
			FLASH_SR_EOP |
			FLASH_SR_WRPRTERR |
			FLASH_SR_BSY));
	}

	return flags;
}

//  Based on https://github.com/libopencm3/libopencm3/blob/master/lib/stm32/common/flash_common_f01.c

void base_flash_wait_for_last_operation(void)
{
	while ((base_flash_get_status_flags() & FLASH_SR_BSY) == FLASH_SR_BSY);
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

void base_flash_program_half_word(uint32_t address, uint16_t data)
{
	base_flash_wait_for_last_operation();

	if ((DESIG_FLASH_SIZE > 512) && (address >= FLASH_BASE+0x00080000)) {
		FLASH_CR2 |= FLASH_CR_PG;
	} else {
		FLASH_CR |= FLASH_CR_PG;
	}

	MMIO16(address) = data;

	base_flash_wait_for_last_operation();

	if ((DESIG_FLASH_SIZE > 512) && (address >= FLASH_BASE+0x00080000)) {
		FLASH_CR2 &= ~FLASH_CR_PG;
	} else {
		FLASH_CR &= ~FLASH_CR_PG;
	}
}

static uint16_t* get_flash_end(void) {
#ifdef FLASH_SIZE_OVERRIDE
    /* Allow access to the flash size we define */
    return (uint16_t*)(FLASH_BASE + FLASH_SIZE_OVERRIDE);
#else
    /* Only allow access to the chip's self-reported flash size */
    return (uint16_t*)(FLASH_BASE + (size_t)DESIG_FLASH_SIZE*FLASH_PAGE_SIZE);
#endif
}

static inline uint16_t* get_flash_page_address(uint16_t* dest) {
    return (uint16_t*)(((uint32_t)dest / FLASH_PAGE_SIZE) * FLASH_PAGE_SIZE);
}

bool base_flash_program_array(uint16_t* dest, const uint16_t* data, size_t half_word_count) {
    bool verified = true;

    /* Remember the bounds of erased data in the current page */
    static uint16_t* erase_start;
    static uint16_t* erase_end;

    const uint16_t* flash_end = get_flash_end();
    debug_print("target_flash "); debug_printhex_unsigned((size_t) dest); ////
    //  debug_print(", data "); debug_printhex_unsigned((size_t) data); 
    debug_print(" to "); debug_printhex_unsigned((size_t) flash_end); 
    debug_print(", hlen "); debug_printhex_unsigned((size_t) half_word_count); 
    debug_println(""); ////
    while (half_word_count > 0) {
        /* Avoid writing past the end of flash */
        if (dest >= flash_end) {
            //  TODO: Fails here
            debug_println("dest >= flash_end"); debug_flush();
            verified = false;
            break;
        }

        if (dest >= erase_end || dest < erase_start) {
            erase_start = get_flash_page_address(dest);
            erase_end = erase_start + (FLASH_PAGE_SIZE)/sizeof(uint16_t);
            flash_erase_page((uint32_t)erase_start);
        }
        flash_program_half_word((uint32_t)dest, *data);
        erase_start = dest + 1;
        if (*dest != *data) {
            debug_println("*dest != *data"); debug_flush();
            verified = false;
            break;
        }
        dest++;
        data++;
        half_word_count--;
    }

    return verified;
}

#define ROM_START 0x08000000
#define ROM_SIZE     0x10000

void baseloader_start(void) {
	//  Disable interrupts because the vector table may be overwritten during flashing.
    cm_disable_interrupts();

	//  Test the baseloader: Copy a page from low flash memory to high flash memory.
	uint32_t *src = (uint32_t *) ROM_START;
	uint32_t *dest = (uint32_t *) ROM_START + ROM_SIZE - FLASH_PAGE_SIZE;

    debug_print("src  "); debug_printhex_unsigned((size_t) src); debug_println("");
    debug_print("dest "); debug_printhex_unsigned((size_t) dest); debug_println("");
    debug_print("before "); debug_printhex_unsigned(*dest); debug_println(""); debug_flush();

	base_flash_unlock();
	bool ok = base_flash_program_array((uint16_t *) dest, (uint16_t *) src, FLASH_PAGE_SIZE / 2);
	base_flash_lock();

    debug_print("after "); debug_printhex_unsigned(*dest);
    debug_print(" / "); debug_printhex(ok); debug_println("\r\n"); debug_flush();

	src = (uint32_t *) ROM_START + FLASH_PAGE_SIZE;

    debug_print("src  "); debug_printhex_unsigned((size_t) src); debug_println("");
    debug_print("dest "); debug_printhex_unsigned((size_t) dest); debug_println("");
    debug_print("before "); debug_printhex_unsigned(*dest); debug_println(""); debug_flush();

	base_flash_unlock();
	ok = base_flash_program_array((uint16_t *) dest, (uint16_t *) src, FLASH_PAGE_SIZE / 2);
	base_flash_lock();

    debug_print("after "); debug_printhex_unsigned(*dest);
    debug_print(" / "); debug_printhex(ok); debug_println("\r\n"); debug_flush();

	//  Vector table may be overwritten. Restart to use the new vector table.
    //  TODO: scb_reset_system();
	//  Should not return.
	for (;;) {}
}
