//  Baseloader Functions
#ifndef BASELOADER_H_INCLUDED
#define BASELOADER_H_INCLUDED
#include <stdint.h>

#ifdef __cplusplus
extern "C" {  //  Allows functions below to be called by C and C++ code.
#endif

#define BOOTLOADER_VERSION ((uint32_t) 0x00010001) 

#define BASE_MAGIC_NUMBER  ((uint32_t) 0x22051969)

typedef int (*baseloader_func)(uint32_t *dest, const uint32_t *src, size_t byte_count);
typedef void (*application_func)(void);

//  Baseloader Vector Table. Located just after STM32 Vector Table.
typedef struct {
	uint32_t magic_number;			//  Magic number to verify this as a Baseloader Vector Table: 0x22051969
	uint32_t version;				//  Bootloader version number e.g. 0x 00 01 00 01 for 1.01
	baseloader_func baseloader;		//  Address of the Baseloader function in ROM.
	uint32_t *baseloader_end;       //  End of Baseloader ROM (code and data).
	application_func application;	//  Start address of Application ROM. Also where the bootloader ends.
} __attribute__((packed)) base_vector_table_t;

extern int baseloader_start(uint32_t *dest, const uint32_t *src, size_t byte_count);
extern int baseloader_fetch(baseloader_func *baseloader_addr, uint32_t **dest, const uint32_t **src, size_t *byte_count);
// extern bool base_flash_program_array(uint16_t* dest0, const uint16_t* src0, size_t half_word_count0);

extern void test_copy_bootloader(void);
extern void test_copy_baseloader(void);
extern void test_copy_end(void);
extern void test_baseloader1(void);
extern void test_baseloader2(void);
extern void test_baseloader_end(void);

#ifdef __cplusplus
}  //  End of extern C scope.
#endif

#endif  //  BASELOADER_H_INCLUDED
