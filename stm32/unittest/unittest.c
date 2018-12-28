//  Unit Test
#include "unittest.h"

#ifdef UNIT_TEST  //  Include the unit test cases.
#include "../../lib/nano-float/test/test.c"
#endif  //  UNIT_TEST

int run_unit_test(void) {
#ifdef UNIT_TEST
    return test_nanofloat();
#else
    return -1;  //  No unit tests.
#endif  //  UNIT_TEST
}
