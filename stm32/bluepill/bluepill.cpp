//  Defines functions specific to the STM32 Blue Pill platform.
#include <libopencm3/cm3/cortex.h>
#include <libopencm3/stm32/rcc.h>
#include <logger.h>
#include "bluepill.h"

extern "C" void test_codal(); ////

//  Debugging is off by default.  Developer must switch it on with enable_debug().
static bool debugEnabled = false;

void platform_setup(void) {
    //  Initialise the STM32 platform. At startup, the onboard LED will blink on-off-on-off-on and stays on.
	//  If LED blinks on-off-on-off and stays off, then debug mode is enabled and no debugger is connected.
	rcc_clock_setup_in_hse_8mhz_out_72mhz();
	led_setup();
	if (debugEnabled) {
		led_on(); led_wait();
		led_off(); led_wait();
		led_on(); led_wait();
		led_off(); led_wait();
		//  This line will call ARM Semihosting and may hang until debugger is connected.
  		debug_println("----platform_setup");
		led_on();
	}

	////TODO
	test_codal();
}

void enable_debug(void) {
	//  Enable ARM Semihosting for displaying debug messages.
	debugEnabled = true;
	enable_log();
}

void disable_debug(void) {
	//  Disable ARM Semihosting for displaying debug messages.
	debugEnabled = false;
	disable_log();
}

//  For Legacy Arduino Support only...

#define ALL_PINS_SIZE (sizeof(allPins) / sizeof(uint32_t))
static const uint32_t allPins[] = {  //  Map Arduino pin number to STM32 port ID.
	0,  //  Unknown pin.
	SPI1,
	SPI2,
	I2C1,
	I2C2,
	USART1,
	USART2,
};  //  TODO: Support STM32 alternate port mapping.

uint32_t convert_pin_to_port(uint8_t pin) {
	//  Map Arduino Pin to STM32 Port, e.g. 1 becomes SPI1
	if (pin < 1 || pin >= ALL_PINS_SIZE) { return 0; }  //  Invalid pin.
	return allPins[pin];
}

uint8_t convert_port_to_pin(uint32_t port_id) {
	//  Map STM32 port to Arduino Pin, e.g. SPI1 becomes 1
	for (uint8_t pin = 1; pin < ALL_PINS_SIZE; pin++) {
		if (port_id == allPins[pin]) { return pin; }
	}
	return 0;  //  Invalid port.
}

///////////////////////////////////////////////////////////////////////////////
////TODO: Testing Codal

#include <codal_target_hal.h>

void test_codal() {
	target_reset();
}

void target_reset() {
	//  TODO
  	debug_println("----target_reset"); debug_flush();
}

void target_enable_irq() {
	//  TODO
  	debug_println("----target_enable_irq"); debug_flush();
}

void target_disable_irq() {
	//  TODO
  	debug_println("----target_disable_irq"); debug_flush();
}

void target_panic(int statusCode) {
	//  TODO
	debug_println("*****target_panic ");
	debug_println((int) statusCode);
	debug_flush();
	for (;;) {}  //  Loop forever.
}

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