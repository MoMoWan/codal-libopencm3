//  Baseloader Functions
#ifndef BASELOADER_H_INCLUDED
#define BASELOADER_H_INCLUDED

#ifdef __cplusplus
extern "C" {  //  Allows functions below to be called by C and C++ code.
#endif

#define BOOTLOADER_VERSION ((uint32_t) 0x00010001) 

#define BASE_MAGIC_NUMBER  ((uint32_t) 0x22051969)

typedef void (*base_vector_table_entry_t)(void);

//  Baseloader Vector Table. Located just after STM32 Vector Table.

typedef struct {
	uint32_t magic_number;					//  Magic number to verify this as a Baseloader Vector Table.
	uint32_t version;						//  Bootloader version number e.g. 0x 00 01 00 01 for 1.01.
	base_vector_table_entry_t baseloader;	//  Address of the baseloader function.
	base_vector_table_entry_t application;	//  Address of application. Also where the bootloader ends.
} __attribute__((packed)) base_vector_table_t;

void baseloader_start(void);

#ifdef __cplusplus
}  //  End of extern C scope.
#endif

#endif  //  BASELOADER_H_INCLUDED
