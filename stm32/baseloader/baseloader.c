#include <libopencm3/stm32/desig.h>  //  For DESIG_FLASH_SIZE
#include <libopencm3/stm32/flash.h>
#include "baseloader.h"

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

// boot_target_flash_unlock();
// bool ok = boot_target_flash_program_array((void *)flashAddr, (void*)flashBuf, FLASH_PAGE_SIZE / 2);
// boot_target_flash_lock();

void baseloader_start(void) {
    //  TODO: cm_disable_interrupts();
    //  TODO: scb_reset_system();
}

