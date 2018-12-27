//  We provide our own main() function to do our own startup after the constructors have been called.
//  Then we call the provided main() function, which has been wrapped as __wrap_main() by the linker.
#include <platform_includes.h>

extern "C" int __wrap_main(void);
extern "C" int test_nanofloat(void);

extern "C" int main(void) {
    //  Note: Must disable debug when testing Deep Sleep.  Else device will not run without ST Link.
    target_enable_debug();       //  Uncomment to allow display of debug messages in development devices. NOTE: This will hang if no Arm Semihosting debugger is attached.
    //  target_disable_debug();  //  Uncomment to disable display of debug messages.  For use in production devices.
    target_init();               //  Init the STM32 platform, which calls the bootloader.  If the bootloader decides to launch the firmware, this function will not return.

#ifdef UNIT_TEST
    //  Run the unit tests if any.
    test_nanofloat();
#endif

    //  Call the wrapped main().
    int status = __wrap_main();
    return status;
}
