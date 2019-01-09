//  Flash Constants for Blue Pill
#ifndef FLASH_CONFIG_H_INCLUDED
#define FLASH_CONFIG_H_INCLUDED

#ifdef __cplusplus
extern "C" {  //  Allows functions below to be called by C and C++ code.
#endif

extern uint32_t _text;        //  Defined by the linker script where the application starts running.
extern uint32_t _base_etext;  //  End of baseloader code and data, defined in the linker script.
#define APP_BASE_ADDRESS      ((uint32_t) &_text)     //  Application starts here, after the bootloader.
#define FLASH_SIZE_OVERRIDE   0x10000                 //  Assume 64KB of ROM, don't override to 128 KB.
#define FLASH_PAGE_SIZE       1024                    
#define FLASH_PAGE_HALF_WORD_COUNT (FLASH_PAGE_SIZE / 2)  //  Number of half-words (16 bits) in a flash page.

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

#ifdef __cplusplus
}  //  End of extern C scope.
#endif

#endif  //  FLASH_CONFIG_H_INCLUDED
