//  Unit Test for nano-float:
//  cd /mnt/c/qemu_stm32 ; arm-softmmu/qemu-system-arm -M stm32-f103c8 -semihosting -kernel /mnt/c/codal-libopencm3/.pioenvs/bluepill_f103c8/firmware.bin
//  cd C:\qemu_stm32 && arm-none-eabi-gdb -ex="target remote localhost:1234" /codal-libopencm3/.pioenvs/bluepill_f103c8/firmware.elf
#ifdef UNIT_TEST
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/rtc.h>
#include <unity.h>
#include <logger.h>
#include <math.h>

void test_sqrt(void) {
    //  TEST_ASSERT_DOUBLE_WITHIN
    TEST_ASSERT_EQUAL_DOUBLE(10.000000, sqrt(100));
    TEST_ASSERT_EQUAL_DOUBLE(1.414214, sqrt(2));
    TEST_ASSERT_EQUAL_DOUBLE(0.000000, sqrt(0));
}

int test_nanofloat(void) {
    UNITY_BEGIN();
    RUN_TEST(test_sqrt);
    int fails = UNITY_END();
    debug_flush();
    //  Crash and exit.
    rtc_awake_from_off(LSE);
    return fails;
}

void unity_output_char(int ch) {
    debug_write((uint8_t) ch);
}
#endif  //  UNIT_TEST
