//  Rewrite standard C math functions with qfplib to reduce ROM size.
#include <math.h>
#include <qfplib.h>

//  Run-time ABI for the ARM Architecture.  The function names are wrapped via "-Wl,-wrap,__aeabi..."
//  in newlib/CMakeLists.txt.  See http://infocenter.arm.com/help/topic/com.arm.doc.ihi0043d/IHI0043D_rtabi.pdf

//  double to integer C-style conversion
int __wrap___aeabi_d2iz(double x)  { return qfp_float2int(x); }

///////////////////////////////////////////////////////////////////////////////
//  Table 2, Standard double precision floating-point arithmetic helper functions

//  double-precision division, n / d
double __wrap___aeabi_ddiv(double n, double d) { return qfp_fdiv_fast(n, d); }

//  double-precision multiplication
double __wrap___aeabi_dmul(double x, double y) { return qfp_fmul(x, y); }

///////////////////////////////////////////////////////////////////////////////
//  Table 3, double precision floating-point comparison helper functions

//  result (1, 0) denotes (=, ?<>) [2], use for C == and !=
int __wrap___aeabi_dcmpeq(double x, double y) { return qfp_fcmp(x, y); }

//  result (1, 0) denotes (<, ?>=) [2], use for C <
int __wrap___aeabi_dcmplt(double x, double y) { return qfp_fcmp(x, y); }

//  result (1, 0) denotes (<=, ?>) [2], use for C <=
int __wrap___aeabi_dcmple(double x, double y) { return qfp_fcmp(x, y); }

//  result (1, 0) denotes (>=, ?<) [2], use for C >=
int __wrap___aeabi_dcmpge(double x, double y) { return qfp_fcmp(x, y); }

//  result (1, 0) denotes (>, ?<=) [2], use for C >
int __wrap___aeabi_dcmpgt(double x, double y) { return qfp_fcmp(x, y); }

//  result (1, 0) denotes (?, <=>) [2], use for C99 isunordered()
int __wrap___aeabi_dcmpun(double x, double y) { return qfp_fcmp(x, y); }

///////////////////////////////////////////////////////////////////////////////
//  Table 4, Standard single precision floating-point arithmetic helper functions

//  single-precision division, n / d
float  __wrap___aeabi_fdiv(float  n, float d)  { return qfp_fdiv_fast(n, d); }

///////////////////////////////////////////////////////////////////////////////
//  Table 6, Standard floating-point to integer conversions

//  double to unsigned C-style conversion
unsigned __wrap___aeabi_d2uiz(double x) { return qfp_float2uint(x); }

///////////////////////////////////////////////////////////////////////////////
//  <math.h> Functions

// CMakeFiles/STM32_BLUE_PILL.dir/pxtapp/base/core.cpp.o: In function `Math_::sqrt(pxt::TValueStruct*)':
// /src/pxtapp/base/core.cpp:925: undefined reference to `sqrt'

double sqrt(double x) { return qfp_fsqrt_fast(x); }

// CMakeFiles/STM32_BLUE_PILL.dir/pxtapp/base/core.cpp.o: In function `Math_::log(pxt::TValueStruct*)':
// /src/pxtapp/base/core.cpp:901: undefined reference to `log'

double log(double x) { return qfp_fln(x); }
double exp(double x) { return qfp_fexp(x); }

// CMakeFiles/STM32_BLUE_PILL.dir/pxtapp/base/core.cpp.o: In function `Math_::log10(pxt::TValueStruct*)':
// /src/pxtapp/base/core.cpp:904: undefined reference to `log10'

double log10(double x) { return qfp_fln(x) / qfp_fln(10); }

//  pow(b, x) = pow(e, log(b) * x) = exp(log(b) * x)
//  e.g. pow(10, 3) = exp(log(10) * 3)

double pow(double b, double x) { return qfp_fexp(qfp_fln(b) * x); }

// CMakeFiles/STM32_BLUE_PILL.dir/pxtapp/base/core.cpp.o: In function `Math_::sin(pxt::TValueStruct*)':
// /src/pxtapp/base/core.cpp:910: undefined reference to `sin'

double sin(double x) { return qfp_fsin(x); }

// CMakeFiles/STM32_BLUE_PILL.dir/pxtapp/base/core.cpp.o: In function `Math_::cos(pxt::TValueStruct*)':
// /src/pxtapp/base/core.cpp:913: undefined reference to `cos'

double cos(double x) { return qfp_fcos(x); }

// CMakeFiles/STM32_BLUE_PILL.dir/pxtapp/base/core.cpp.o: In function `Math_::tan(pxt::TValueStruct*)':
// /src/pxtapp/base/core.cpp:907: undefined reference to `tan'

double tan(double x) { return qfp_ftan(x); }

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

// CMakeFiles/STM32_BLUE_PILL.dir/pxtapp/base/core.cpp.o: In function `Math_::floor(pxt::TValueStruct*)':
// /src/pxtapp/base/core.cpp:928: undefined reference to `floor'

////  double floor(double x) { return (x); }

// CMakeFiles/STM32_BLUE_PILL.dir/pxtapp/base/core.cpp.o: In function `Math_::ceil(pxt::TValueStruct*)':
// /src/pxtapp/base/core.cpp:931: undefined reference to `ceil'

////  double ceil(double x) { return (x); }

// CMakeFiles/STM32_BLUE_PILL.dir/pxtapp/base/core.cpp.o: In function `Math_::trunc(pxt::TValueStruct*)':
// /src/pxtapp/base/core.cpp:934: undefined reference to `trunc'

////  double trunc(double x) { return (x); }

//  double fmod(double, double);

//  TODO
//  double cosh(double x) { return (x); }
//  double sinh(double x) { return (x); }
//  double ldexp(double, int);
