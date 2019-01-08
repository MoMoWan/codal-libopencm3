//  Baseloader Functions
#ifndef BASELOADER_H_INCLUDED
#define BASELOADER_H_INCLUDED

#ifdef __cplusplus
extern "C" {  //  Allows functions below to be called by C and C++ code.
#endif

#define BASE_MAGIC_NUMBER ((uint32_t) 0x22051969)

void baseloader_start(void);

#ifdef __cplusplus
}  //  End of extern C scope.
#endif

#endif  //  BASELOADER_H_INCLUDED
