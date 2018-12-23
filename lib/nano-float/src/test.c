//  Unit Test for nano-float
#ifdef UNIT_TEST
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
    return UNITY_END();
}

void unity_output_char(int ch) {
    debug_write((uint8_t) ch);
}
#endif  //  UNIT_TEST
