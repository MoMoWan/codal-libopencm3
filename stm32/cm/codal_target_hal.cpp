#include "stm32.h"
#include "codal_target_hal.h"
#include "CodalDmesg.h"
#include "CodalCompat.h"
#include <logger.h>

extern "C" void test_codal(); ////

//  TODO: From https://github.com/mmoskal/codal-generic-f103re/blob/master/source/codal_target_hal.cpp

//  Blue Pill Memory Layout: ~/.platformio/packages/framework-libopencm3/lib/stm32/f1/stm32f103x8.ld
//  RAM Layout: [Start of RAM] [Data] [BSS] [Codal Heap] grows--> (empty) <--grows [Stack] [End of RAM]

extern PROCESSOR_WORD_TYPE _ebss;  //  End of BSS.
PROCESSOR_WORD_TYPE codal_heap_start = (PROCESSOR_WORD_TYPE)(&_ebss);

//  TODO: From https://github.com/lancaster-university/codal-arduino-uno/blob/master/source/codal_target_hal.cpp

extern "C" void __cxa_pure_virtual() {
	//  Disable exceptions for abstract classes. See https://arobenko.gitbooks.io/bare_metal_cpp/content/compiler_output/abstract_classes.html
    target_panic(1000);
}

// define new and delete.
extern "C" void *operator new(size_t objsize) {
    return malloc(objsize);
}

extern "C" void operator delete(void* obj) {
    free(obj);
}

void test_codal() {
	PROCESSOR_WORD_TYPE start = (PROCESSOR_WORD_TYPE)(codal_heap_start); 
	PROCESSOR_WORD_TYPE end = (PROCESSOR_WORD_TYPE)(DEVICE_STACK_BASE) - (PROCESSOR_WORD_TYPE)(DEVICE_STACK_SIZE);
	PROCESSOR_WORD_TYPE size = end - start;
	debug_print("heap start: "); debug_println((size_t) start);
	debug_print("heap end: "); debug_println((size_t) end);
	debug_print("heap size: "); debug_println((size_t) size);
	target_reset();
}

void target_enable_irq()
{
	//  TODO
  	debug_println("----target_enable_irq"); debug_flush();
    ////__enable_irq();
}

void target_disable_irq()
{
	//  TODO
  	debug_println("----target_disable_irq"); debug_flush();
    ////__disable_irq();
}

void target_wait_for_event()
{
    //  TODO
    ////__WFE();
}

void target_wait(uint32_t milliseconds)
{
    //  TODO
    ////HAL_Delay(milliseconds);
}

extern void wait_us(uint32_t);
void target_wait_us(unsigned long us)
{
    wait_us(us);
}

int target_seed_random(uint32_t rand)
{
    //  TODO
    return 0;  ////  TODO
    ////return codal::seed_random(rand);
}

int target_random(int max)
{
    //  TODO
    return 0;  ////  TODO
    ////return codal::random(max);
}

/*
    The unique device identifier is ideally suited:
        * for use as serial numbers (for example USB string serial numbers or other end applications)
        * for use as security keys in order to increase the security of code in Flash memory while using and combining this unique ID with software cryptographic primitives and protocols before programming the internal Flash memory
        * to activate secure boot processes, etc.
    The 96-bit unique device identifier provides a reference number which is unique for any
    device and in any context. These bits can never be altered by the user.
    The 96-bit unique device identifier can also be read in single bytes/half-words/words in different ways and then be concatenated using a custom algorithm.
*/
#define STM32_UUID ((uint32_t *)0x1FFF7A10)
uint32_t target_get_serial()
{
    // uuid[1] is the wafer number plus the lot number, need to check the uniqueness of this...
    return (uint32_t)STM32_UUID[1];
}

void target_reset()
{
	//  TODO
  	debug_println("----target_reset"); debug_flush();
#ifdef TODO
    PWR->CR |= PWR_CR_DBP;
    RCC->BDCR |= RCC_BDCR_RTCEN;
    RTC->BKP0R = 0x24a22d12; // skip bootloader
    NVIC_SystemReset();
#endif  //  TODO
}

extern "C" void assert_failed(uint8_t* file, uint32_t line)
{
    target_panic(920);
}

__attribute__((weak))
void target_panic(int statusCode)
{
	//  TODO
    target_disable_irq();
	debug_println("*****target_panic ");
	debug_println((int) statusCode);
	debug_flush();
    ////DMESG("*** CODAL PANIC : [%d]", statusCode);
	for (;;) {}  //  Loop forever.
}

extern "C" void init_irqs();

void target_init()
{
    //  TODO
    ////HAL_Init();
    init_irqs();
}

/**
 *  Thread Context for an ARM Cortex core.
 *
 * This is probably overkill, but the ARMCC compiler uses a lot register optimisation
 * in its calling conventions, so better safe than sorry!
 */
struct PROCESSOR_TCB
{
    uint32_t R0;
    uint32_t R1;
    uint32_t R2;
    uint32_t R3;
    uint32_t R4;
    uint32_t R5;
    uint32_t R6;
    uint32_t R7;
    uint32_t R8;
    uint32_t R9;
    uint32_t R10;
    uint32_t R11;
    uint32_t R12;
    uint32_t SP;
    uint32_t LR;
    uint32_t stack_base;
};

PROCESSOR_WORD_TYPE fiber_initial_stack_base()
{
    uint32_t mbed_stack_base;

    mbed_stack_base = (PROCESSOR_WORD_TYPE) DEVICE_STACK_BASE;

    return mbed_stack_base;
}

void *tcb_allocate()
{
    return (void *)malloc(sizeof(PROCESSOR_TCB));
}

/**
 * Configures the link register of the given tcb to have the value function.
 *
 * @param tcb The tcb to modify
 * @param function the function the link register should point to.
 */
void tcb_configure_lr(void *tcb, PROCESSOR_WORD_TYPE function)
{
    PROCESSOR_TCB *tcbPointer = (PROCESSOR_TCB *)tcb;
    tcbPointer->LR = function;
}

/**
 * Configures the link register of the given tcb to have the value function.
 *
 * @param tcb The tcb to modify
 * @param function the function the link register should point to.
 */
void tcb_configure_sp(void *tcb, PROCESSOR_WORD_TYPE sp)
{
    PROCESSOR_TCB *tcbPointer = (PROCESSOR_TCB *)tcb;
    tcbPointer->SP = sp;
}

void tcb_configure_stack_base(void *tcb, PROCESSOR_WORD_TYPE stack_base)
{
    PROCESSOR_TCB *tcbPointer = (PROCESSOR_TCB *)tcb;
    tcbPointer->stack_base = stack_base;
}

PROCESSOR_WORD_TYPE tcb_get_stack_base(void *tcb)
{
    PROCESSOR_TCB *tcbPointer = (PROCESSOR_TCB *)tcb;
    return tcbPointer->stack_base;
}

PROCESSOR_WORD_TYPE get_current_sp()
{
    return __get_MSP();
}

PROCESSOR_WORD_TYPE tcb_get_sp(void *tcb)
{
    PROCESSOR_TCB *tcbPointer = (PROCESSOR_TCB *)tcb;
    return tcbPointer->SP;
}

void tcb_configure_args(void *tcb, PROCESSOR_WORD_TYPE ep, PROCESSOR_WORD_TYPE cp,
                        PROCESSOR_WORD_TYPE pm)
{
    PROCESSOR_TCB *tcbPointer = (PROCESSOR_TCB *)tcb;
    tcbPointer->R0 = (uint32_t)ep;
    tcbPointer->R1 = (uint32_t)cp;
    tcbPointer->R2 = (uint32_t)pm;
}
