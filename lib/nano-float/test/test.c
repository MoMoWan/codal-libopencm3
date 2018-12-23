//  Unit Test for nano-float:
//  cd /mnt/c/qemu_stm32 ; arm-softmmu/qemu-system-arm -M stm32-f103c8 -semihosting -kernel /mnt/c/codal-libopencm3/.pioenvs/bluepill_f103c8/firmware.bin
//  cd C:\qemu_stm32 && arm-none-eabi-gdb -ex="target remote localhost:1234" /codal-libopencm3/.pioenvs/bluepill_f103c8/firmware.elf
#ifdef UNIT_TEST
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/rtc.h>
#include <unity.h>
#include <logger.h>
#include <math.h>

uint8_t *get_float_usage(uint16_t *size);
volatile double x = 0, y = 0, r = 0;

void test_sqrt(void) {
    //  TODO: TEST_ASSERT_DOUBLE_WITHIN
    //  Must use x because compiler will optimise all constants.
    x = 100; r = sqrt(x); TEST_ASSERT_EQUAL_DOUBLE(10.000000, r);
    x = 2; TEST_ASSERT_EQUAL_DOUBLE(1.414214, sqrt(x));
    x = 0; TEST_ASSERT_EQUAL_DOUBLE(0.000000, sqrt(x));
}

void test_aeabi_ddiv(void) {
    x = 2205.1969; y = 270.8886; r = x / y; TEST_ASSERT_EQUAL_DOUBLE( 8.140604292687105, r );
}

void test_aeabi_dmul(void) {
    x = 2205.1969; y = 270.8886; r = x * y; TEST_ASSERT_EQUAL_DOUBLE( 597362.70096534, r );
}

int test_nanofloat(void) {
    debug_flush();
    UNITY_BEGIN();

    RUN_TEST(test_sqrt);
    RUN_TEST(test_aeabi_ddiv);
    RUN_TEST(test_aeabi_dmul);

    int fails = UNITY_END(); debug_flush();
    uint16_t size = 0;
    uint8_t *float_usage = get_float_usage(&size);
    if (float_usage != NULL && size < 1000) {
        uint16_t i = 0;
        for (i = 0; i < size; i++) {
            if (float_usage[i] == 0) { continue; }
            debug_printhex(i); debug_print(" > "); 
            debug_printhex(float_usage[i]); debug_print(" / ");
        }
        debug_println(""); debug_flush();
    }
    //  Crash and exit.
    rtc_awake_from_off(LSE);
    return fails;
}

void unity_output_char(int ch) {
    debug_write((uint8_t) ch);
}
#endif  //  UNIT_TEST
