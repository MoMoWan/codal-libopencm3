//  Rewrite standard C math functions with qfplib to reduce ROM size.
#include <math.h>
#include <qfplib.h>

// CMakeFiles/STM32_BLUE_PILL.dir/pxtapp/base/core.cpp.o: In function `Math_::log(pxt::TValueStruct*)':
// /src/pxtapp/base/core.cpp:901: undefined reference to `log'

double log(double x) { return qfp_fln(x); }

// CMakeFiles/STM32_BLUE_PILL.dir/pxtapp/base/core.cpp.o: In function `Math_::log10(pxt::TValueStruct*)':
// /src/pxtapp/base/core.cpp:904: undefined reference to `log10'

double log10(double x) { return qfp_fln(x) / qfp_fln(10); }

// CMakeFiles/STM32_BLUE_PILL.dir/pxtapp/base/core.cpp.o: In function `Math_::tan(pxt::TValueStruct*)':
// /src/pxtapp/base/core.cpp:907: undefined reference to `tan'

double tan(double x) { return qfp_ftan(x); }

// CMakeFiles/STM32_BLUE_PILL.dir/pxtapp/base/core.cpp.o: In function `Math_::sin(pxt::TValueStruct*)':
// /src/pxtapp/base/core.cpp:910: undefined reference to `sin'

double sin(double x) { return qfp_fsin(x); }

// CMakeFiles/STM32_BLUE_PILL.dir/pxtapp/base/core.cpp.o: In function `Math_::cos(pxt::TValueStruct*)':
// /src/pxtapp/base/core.cpp:913: undefined reference to `cos'

double cos(double x) { return qfp_fcos(x); }

// CMakeFiles/STM32_BLUE_PILL.dir/pxtapp/base/core.cpp.o: In function `Math_::atan(pxt::TValueStruct*)':
// /src/pxtapp/base/core.cpp:916: undefined reference to `atan'

////  double atan(double x) { return (x); }
double atan2(double x, double y) { return qfp_fatan2(x, y); }

// CMakeFiles/STM32_BLUE_PILL.dir/pxtapp/base/core.cpp.o: In function `Math_::asin(pxt::TValueStruct*)':
// /src/pxtapp/base/core.cpp:919: undefined reference to `asin'

////  double asin(double x) { return (x); }

// CMakeFiles/STM32_BLUE_PILL.dir/pxtapp/base/core.cpp.o: In function `Math_::acos(pxt::TValueStruct*)':
// /src/pxtapp/base/core.cpp:922: undefined reference to `acos'

////  double acos(double x) { return (x); }

// CMakeFiles/STM32_BLUE_PILL.dir/pxtapp/base/core.cpp.o: In function `Math_::sqrt(pxt::TValueStruct*)':
// /src/pxtapp/base/core.cpp:925: undefined reference to `sqrt'

double sqrt(double x) { return qfp_fsqrt_fast(x); }

// CMakeFiles/STM32_BLUE_PILL.dir/pxtapp/base/core.cpp.o: In function `Math_::floor(pxt::TValueStruct*)':
// /src/pxtapp/base/core.cpp:928: undefined reference to `floor'

////  double floor(double x) { return (x); }

// CMakeFiles/STM32_BLUE_PILL.dir/pxtapp/base/core.cpp.o: In function `Math_::ceil(pxt::TValueStruct*)':
// /src/pxtapp/base/core.cpp:931: undefined reference to `ceil'

////  double ceil(double x) { return (x); }

// CMakeFiles/STM32_BLUE_PILL.dir/pxtapp/base/core.cpp.o: In function `Math_::trunc(pxt::TValueStruct*)':
// /src/pxtapp/base/core.cpp:934: undefined reference to `trunc'

////  double trunc(double x) { return (x); }

//  pow(b, x) = pow(e, log(b) * x) = exp(log(b) * x)
//  e.g. pow(10, 3) = exp(log(10) * 3)

double pow(double b, double x) { return qfp_fexp(qfp_fln(b) * x); }

//double cosh(double x) { return (x); }
//double sinh(double x) { return (x); }
//double exp(double x) { return (x); }
//double ldexp(double, int);
//double fmod(double, double);
