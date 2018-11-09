# codal-libopencm3
Codal framework ported to libopencm3 to support MakeCode on STM32 Blue Pill

Based on code from:

https://github.com/lupyuen/send_altitude_cocoos

https://github.com/lancaster-university/codal-stm32

https://github.com/mmoskal/codal-generic-f103re

https://github.com/LabAixBidouille-STM32/codal-stm32-iot-node

## Installation

Original instructions: https://lancaster-university.github.io/codal/

Before using this target you need to configure your platforms with some software.
Codal is also a build system to simplify as much as possible the experience of novice users.  

1. Install `git`, ensure it is available on your platforms path.
2. Install the `arm-none-eabi-*` command line utilities for ARM based devices and/or `avr-gcc`, `avr-binutils`, `avr-libc` for AVR based devices, ensure they are available on your platforms path.
3. Install [CMake](https://cmake.org)(Cross platform make), this is the entirety of the build system.
4. Install `Python 2.7` (if you are unfamiliar with CMake), python scripts are used to simplify the build process.
5. Clone the repository <https://github.com/lancaster-university/codal>

## Building

- Create a `codal.json` file. See below for the contents.
  - `python build.py codal-libopencm3` generates a codal.json file for our target
- In the root of this repository type `python build.py` the `-c` option cleans before building.
- The bin file `STM32_BLUE_PILL.bin` and hex file `STM32_BLUE_PILL.hex` will be placed at the location specified by `codal.json`, by default this is the root.
- For troubleshooting, the linker output map is generated in `build/STM32_BLUE_PILL.map`. This is configured in `target.json` and `target-locked.json` in `codal-libopencm3`. 

## codal.json

```json
{
    "target": {
        "url": "https://github.com/lupyuen/codal-libopencm3",
        "type": "git",
        "name": "codal-libopencm3",
        "branch": "master"
    }
}
```

## Codal Build Log

```sh
python build.py codal-libopencm3

codal-libopencm3 is already installed
Set target: codal-libopencm3
Using target-locked.json
Targeting codal-libopencm3
-- The C compiler identification is GNU 7.3.1
-- The CXX compiler identification is GNU 7.3.1
-- Checking whether C compiler has -isysroot
-- Checking whether C compiler has -isysroot - yes
-- Checking whether C compiler supports OSX deployment target flag
-- Checking whether C compiler supports OSX deployment target flag - no
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Detecting C compile features
-- Detecting C compile features - done
-- Checking whether CXX compiler has -isysroot
-- Checking whether CXX compiler has -isysroot - yes
-- Checking whether CXX compiler supports OSX deployment target flag
-- Checking whether CXX compiler supports OSX deployment target flag - no
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- The ASM compiler identification is GNU
-- Found assembler: /usr/local/bin/arm-none-eabi-gcc
Supressing -Wexpansion-to-defined.
-- Adding library path: (/Users/Luppy/codal/libraries)
Installing dependencies...
codal-core is already installed
Using library: codal-libopencm3
CODAL_CORE_DIR: /Users/Luppy/codal/libraries/codal-libopencm3/lib/codal-core
COCOOS_DIR: /Users/Luppy/codal/libraries/codal-libopencm3/lib/cocoOS_5.0.3
BME280_DIR: /Users/Luppy/codal/libraries/codal-libopencm3/lib/BME280
BOOST_DIR: /Users/Luppy/codal/libraries/codal-libopencm3/lib/boost_
STM32_DIR: /Users/Luppy/codal/libraries/codal-libopencm3/stm32
LIBOPENCM3_DIR: /Users/Luppy/codal/libraries/codal-libopencm3/lib/libopencm3
fatal: destination path 'libopencm3' already exists and is not an empty directory.
rm: lib/stm32/common/adc_common_v2.c: No such file or directory
rm: lib/stm32/common/adc_common_v2_multi.c: No such file or directory
rm: lib/stm32/common/crc_v2.c: No such file or directory
rm: lib/stm32/common/crs_common_all.c: No such file or directory
rm: lib/stm32/common/crypto_common_f24.c: No such file or directory
rm: lib/stm32/common/flash_common_f24.c: No such file or directory
rm: lib/stm32/common/dma_common_f24.c: No such file or directory
rm: lib/stm32/common/hash_common_f24.c: No such file or directory
rm: lib/stm32/common/timer_common_f24.c: No such file or directory
rm: lib/stm32/common/gpio_common_f0234.c: No such file or directory
rm: lib/stm32/common/rtc_common_l1f024.c: No such file or directory
rm: lib/stm32/common/timer_common_f0234.c: No such file or directory
rm: lib/stm32/common/i2c_common_v2.c: No such file or directory
rm: lib/stm32/common/pwr_common_v2.c: No such file or directory
rm: lib/stm32/common/spi_common_v2.c: No such file or directory
rm: lib/stm32/common/usart_common_v2.c: No such file or directory
rm: lib/stm32/common/flash_common_l01.c: No such file or directory
rm: lib/stm32/common/spi_common_v1_frf.c: No such file or directory
rm: lib/stm32/common/rng_common_v1.c: No such file or directory
CODAL_LIBOPENCM3_DIR: /Users/Luppy/codal/libraries/codal-libopencm3/src
TOP_LEVEL_INCLUDE_DIRS: /Users/Luppy/codal/libraries/codal-libopencm3/src
TOP_LEVEL_SOURCE_FILES: /Users/Luppy/codal/libraries/codal-libopencm3/src/sensor_display.cpp;/Users/Luppy/codal/libraries/codal-libopencm3/asm/CortexContextSwitch.s
CODAL_CORE_INC: /Users/Luppy/codal/libraries/codal-libopencm3/lib/codal-core/inc/core;/Users/Luppy/codal/libraries/codal-libopencm3/lib/codal-core/inc/driver-models;/Users/Luppy/codal/libraries/codal-libopencm3/lib/codal-core/inc/drivers;/Users/Luppy/codal/libraries/codal-libopencm3/lib/codal-core/inc/streams;/Users/Luppy/codal/libraries/codal-libopencm3/lib/codal-core/inc/types
Using library: codal-core
-- Configuring done
-- Generating done
-- Build files have been written to: /Users/Luppy/codal/build
Scanning dependencies of target libopencm3
Scanning dependencies of target bme280
Scanning dependencies of target codal-core
Scanning dependencies of target cocoos
Scanning dependencies of target stm32
[  1%] Building C object libraries/codal-libopencm3/CMakeFiles/cocoos.dir/lib/cocoOS_5.0.3/src/os_cbk.c.o
[  2%] Building C object libraries/codal-libopencm3/CMakeFiles/cocoos.dir/lib/cocoOS_5.0.3/src/os_event.c.o
[  3%] Building C object libraries/codal-libopencm3/CMakeFiles/cocoos.dir/lib/cocoOS_5.0.3/src/os_assert.c.o
[  3%] Building C object libraries/codal-libopencm3/CMakeFiles/cocoos.dir/lib/cocoOS_5.0.3/src/os_kernel.c.o
[  3%] Building CXX object libraries/codal-libopencm3/CMakeFiles/bme280.dir/lib/BME280/src/BME280.cpp.o
[  4%] Building CXX object libraries/codal-libopencm3/CMakeFiles/bme280.dir/lib/BME280/src/BME280Spi.cpp.o
[  5%] Building CXX object libraries/codal-libopencm3/CMakeFiles/bme280.dir/lib/BME280/src/BME280I2C.cpp.o
[  7%] Building CXX object libraries/codal-libopencm3/CMakeFiles/bme280.dir/lib/BME280/src/EnvironmentCalculations.cpp.o
[  7%] Building C object libraries/codal-libopencm3/CMakeFiles/cocoos.dir/lib/cocoOS_5.0.3/src/os_msgqueue.c.o
[ 11%] Building CXX object libraries/codal-core/CMakeFiles/codal-core.dir/source/core/CodalComponent.cpp.o
[ 12%] Building C object libraries/codal-libopencm3/CMakeFiles/libopencm3.dir/lib/libopencm3/lib/cm3/vector.c.o
[  9%] Building C object libraries/codal-libopencm3/CMakeFiles/cocoos.dir/lib/cocoOS_5.0.3/src/os_sem.c.o
[  9%] Building C object libraries/codal-libopencm3/CMakeFiles/cocoos.dir/lib/cocoOS_5.0.3/src/os_task.c.o
[ 10%] Building CXX object libraries/codal-core/CMakeFiles/codal-core.dir/source/core/CodalCompat.cpp.o
/Users/Luppy/codal/libraries/codal-libopencm3/lib/BME280/src/BME280.cpp: In member function 'boo/Users/Luppy/codal/libraries/codal-libopencm3/lib/cocoOS_5.0.3/src/os_msgqueue.c: In function 'queue_push':
[ 13%] Building C object libraries/codal-libopencm3/CMakeFiles/libopencm3.dir/lib/libopencm3/lib/stm32/common/adc_common_v1.c.o
[ 14%] Building CXX object libraries/codal-core/CMakeFiles/codal-core.dir/source/core/CodalDevice.cpp.o
/Users/Luppy/codal/libraries/codal-libopencm3/lib/cocoOS_5.0.3/src/os_msgqueue.c:160:28: warning: cast from pointer to integer of different size [-Wpointer-to-int-cast]
  uint8_t *dst = (uint8_t*)((Mem_t)queue->list + head * msgSz);
                            ^
/Users/Luppy/codal/libraries/codal-libopencm3/lib/cocoOS_5.0.3/src/os_msgqueue.c:160:17: warning: cast to pointer from integer of different size [-Wint-to-pointer-cast]
  uint8_t *dst = (uint8_t*)((Mem_t)queue->list + head * msgSz);
                 ^
[ 15%] Building C object libraries/codal-libopencm3/CMakeFiles/libopencm3.dir/lib/libopencm3/lib/stm32/common/crc_common_all.c.o
[ 16%] Building CXX object libraries/codal-core/CMakeFiles/codal-core.dir/source/core/CodalDmesg.cpp.o
[ 16%] Building C object libraries/codal-libopencm3/CMakeFiles/libopencm3.dir/lib/libopencm3/lib/stm32/common/dac_common_all.c.o
l BME280::WriteSettings()':
/Users/Luppy/codal/libraries/codal-libopencm3/lib/BME280/src/BME280.cpp:98:1: warning: no return statement in function returning non-void [-Wreturn-type]
 }
 ^
/Users/Luppy/codal/libraries/codal-libopencm3/lib/cocoOS_5.0.3/src/os_msgqueue.c: In function 'os_msg_receive':
/Users/Luppy/codal/libraries/codal-libopencm3/lib/cocoOS_5.0.3/src/os_msgqueue.c:199:27: warning: cast from pointer to integer of different size [-Wpointer-to-int-cast]
         src = (uint8_t*)( (Mem_t)q->list + tail * msgSz );
                           ^
[ 17%] Building C object libraries/codal-libopencm3/CMakeFiles/libopencm3.dir/lib/libopencm3/lib/stm32/common/dma_common_l1f013.c.o
[ 17%] Building CXX object libraries/codal-core/CMakeFiles/codal-core.dir/source/core/CodalFiber.cpp.o
/Users/Luppy/codal/libraries/codal-libopencm3/lib/cocoOS_5.0.3/src/os_msgqueue.c:199:15: warning: cast to pointer from integer of different size [-Wint-to-pointer-cast]
         src = (uint8_t*)( (Mem_t)q->list + tail * msgSz );
               ^
/Users/Luppy/codal/libraries/codal-libopencm3/lib/cocoOS_5.0.3/src/os_msgqueue.c:221:31: warning: cast from pointer to integer of different size [-Wpointer-to-int-cast]
             dst = (uint8_t*)( (Mem_t)q->list + q->head * msgSz );
                               ^
[ 19%] Building CXX object libraries/codal-core/CMakeFiles/codal-core.dir/source/core/CodalListener.cpp.o
[ 22%] Building CXX object libraries/codal-libopencm3/CMakeFiles/stm32.dir/stm32/bluepill/STM32BluePill.cpp.o
[ 22%] Building C object libraries/codal-libopencm3/CMakeFiles/libopencm3.dir/lib/libopencm3/lib/stm32/common/flash_common_all.c.o
[ 22%] Linking CXX static library ../../libbme280.a
/Users/Luppy/codal/libraries/codal-libopencm3/lib/cocoOS_5.0.3/src/os_msgqueue.c:221:19: warning: cast to pointer from integer of different size [-Wint-to-pointer-cast]
             dst = (uint8_t*)( (Mem_t)q->list + q->head * msgSz );
                   ^
/Users/Luppy/codal/libraries/codal-libopencm3/lib/cocoOS_5.0.3/src/os_msgqueue.c: In function 'os_msgQ_tick':
 18%] Building CXX object libraries/codal-core/CMakeFiles/codal-core.dir/source/core/CodalHeapAllocator.cpp.o
1m/Users/Luppy/codal/libraries/codal-libopencm3/lib/cocoOS_5.0.3/src/os_msgqueue.c:255:26: warning: cast from pointer to integer of different size [-Wpointer-to-int-cast]
         pMsg = (Msg_t*)( (Mem_t)q->list + nextMessage * msgSz );
                          ^
/Users/Luppy/codal/libraries/codal-libopencm3/lib/cocoOS_5.0.3/src/os_msgqueue.c:255:16: warning: cast to pointer from integer of different size [-Wint-to-pointer-cast]
         pMsg = (Msg_t*)( (Mem_t)q->list + nextMessage * msgSz );
                ^
[ 21%] Building C object libraries/codal-libopencm3/CMakeFiles/libopencm3.dir/lib/libopencm3/lib/stm32/common/exti_common_all.c.o
[ 22%] Building CXX object libraries/codal-core/CMakeFiles/codal-core.dir/source/core/MemberFunctionCallback.cpp.o
[ 20%] Building CXX object libraries/codal-core/CMakeFiles/codal-core.dir/source/core/CodalUtil.cpp.o
[ 22%] Built target bme280
[ 23%] Building CXX object libraries/codal-libopencm3/CMakeFiles/stm32.dir/stm32/bluepill/STM32BluePillIO.cpp.o
[ 24%] Building C object libraries/codal-libopencm3/CMakeFiles/libopencm3.dir/lib/libopencm3/lib/stm32/common/flash_common_f01.c.o
/Users/Luppy/codal/libraries/codal-libopencm3/lib/cocoOS_5.0.3/src/os_msgqueue.c: In function 'MsgQAllDelayed':
/Users/Luppy/codal/libraries/codal-libopencm3/lib/cocoOS_5.0.3/src/os_msgqueue.c:284:26: warning: cast from pointer to integer of different size [-Wpointer-to-int-cast]
         pMsg = (Msg_t*)( (Mem_t)q->list + (nextMessage * msgSz) );
                          ^
/Users/Luppy/codal/libraries/codal-libopencm3/lib/cocoOS_5.0.3/src/os_msgqueue.c:284:16: warning: cast to pointer from integer of different size [-Wint-to-pointer-cast]
         pMsg = (Msg_t*)( (Mem_t)q->list + (nextMessage * msgSz) );
                ^
[ 23%] Building C object libraries/codal-libopencm3/CMakeFiles/libopencm3.dir/lib/libopencm3/lib/stm32/common/flash_common_f.c.o
[ 27%] Building CXX object libraries/codal-libopencm3/CMakeFiles/stm32.dir/stm32/bluepill/bluepill.cpp.o
[ 28%] Building CXX object libraries/codal-core/CMakeFiles/codal-core.dir/source/driver-models/AbstractButton.cpp.o
[ 29%] Building CXX object libraries/codal-libopencm3/CMakeFiles/stm32.dir/stm32/bluepill/led.cpp.o
[ 30%] Building CXX object libraries/codal-core/CMakeFiles/codal-core.dir/source/driver-models/Accelerometer.cpp.o
[ 31%] Building C object libraries/codal-libopencm3/CMakeFiles/libopencm3.dir/lib/libopencm3/lib/stm32/common/gpio_common_all.c.o
[ 31%] Building CXX object libraries/codal-libopencm3/CMakeFiles/stm32.dir/stm32/bluepill/timer.cpp.o
[ 26%] Building C object libraries/codal-libopencm3/CMakeFiles/libopencm3.dir/lib/libopencm3/lib/stm32/common/flash_common_idcache.c.o
[ 25%] Building CXX object libraries/codal-libopencm3/CMakeFiles/stm32.dir/stm32/bluepill/STM32BluePillPin.cpp.o
[ 32%] Building CXX object libraries/codal-core/CMakeFiles/codal-core.dir/source/driver-models/CodalUSB.cpp.o
[ 33%] Building CXX object libraries/codal-libopencm3/CMakeFiles/stm32.dir/stm32/cm/CmI2C.cpp.o
[ 33%] Building CXX object libraries/codal-core/CMakeFiles/codal-core.dir/source/driver-models/Compass.cpp.o
[ 33%] Linking C static library ../../libcocoos.a
[ 33%] Building C object libraries/codal-libopencm3/CMakeFiles/libopencm3.dir/lib/libopencm3/lib/stm32/common/i2c_common_v1.c.o
[ 34%] Building CXX object libraries/codal-core/CMakeFiles/codal-core.dir/source/driver-models/Display.cpp.o
[ 34%] Built target cocoos
[ 35%] Building CXX object libraries/codal-libopencm3/CMakeFiles/stm32.dir/stm32/cm/CmPWM.cpp.o
[ 36%] Building C object libraries/codal-libopencm3/CMakeFiles/libopencm3.dir/lib/libopencm3/lib/stm32/common/iwdg_common_all.c.o
[ 37%] Building CXX object libraries/codal-core/CMakeFiles/codal-core.dir/source/driver-models/Gyroscope.cpp.o
[ 38%] Building C object libraries/codal-libopencm3/CMakeFiles/libopencm3.dir/lib/libopencm3/lib/stm32/common/pwr_common_v1.c.o
[ 39%] Building CXX object libraries/codal-libopencm3/CMakeFiles/stm32.dir/stm32/cm/CmPin.cpp.o
[ 40%] Building CXX object libraries/codal-core/CMakeFiles/codal-core.dir/source/driver-models/I2C.cpp.o
[ 41%] B[ 41%] Building CXX object libraries/codal-core/CMakeFiles/codal-core.dir/source/driver-models/SPI.cpp.o
uilding C object libraries/codal-libopencm3/CMakeFiles/libopencm3.dir/lib/libopencm3/lib/stm32/common/rcc_common_all.c.o
[ 46%] Building CXX object libraries/codal-libopencm3/CMakeFiles/stm32.dir/stm32/cm/CmUSB.cpp.o
[ 41%] Building C object libraries/codal-libopencm3/CMakeFiles/libopencm3.dir/lib/libopencm3/lib/stm32/common/spi_common_all.c.o
[ 41%] Building CXX object libraries/codal-libopencm3/CMakeFiles/stm32.dir/stm32/cm/CmSPI.cpp.o
[ 42%] Building CXX object libraries/codal-libopencm3/CMakeFiles/stm32.dir/stm32/cm/CmSingleWireSerial.cpp.o
[ 43%] Building C object libraries/codal-libopencm3/CMakeFiles/libopencm3.dir/lib/libopencm3/lib/stm32/common/spi_common_v1.c.o
[ 44%] Building CXX object libraries/codal-libopencm3/CMakeFiles/stm32.dir/stm32/cm/CmTimer.cpp.o
[ 46%] Building C object libraries/codal-libopencm3/CMakeFiles/libopencm3.dir/lib/libopencm3/lib/stm32/common/st_usbfs_core.c.o
[ 47%] Building CXX object libraries/codal-core/CMakeFiles/codal-core.dir/source/driver-models/Sensor.cpp.o
[ 47%] Building CXX object libraries/codal-libopencm3/CMakeFiles/stm32.dir/stm32/cm/DynamicPwm.cpp.o
[ 48%] Building CXX object libraries/codal-core/CMakeFiles/codal-core.dir/source/driver-models/Timer.cpp.o
[ 51%] Building CXX object libraries/codal-libopencm3/CMakeFiles/stm32.dir/stm32/cm/codal_target_hal.cpp.o
[ 53%] Building CXX object libraries/codal-core/CMakeFiles/codal-core.dir/source/drivers/AnimatedDisplay.cpp.o
[ 49%] Building C object libraries/codal-libopencm3/CMakeFiles/libopencm3.dir/lib/libopencm3/lib/stm32/common/timer_common_all.c.o
[ 50%] Building CXX object libraries/codal-core/CMakeFiles/codal-core.dir/source/drivers/AnalogSensor.cpp.o
[ 52%] Building C object libraries/codal-libopencm3/CMakeFiles/libopencm3.dir/lib/libopencm3/lib/stm32/common/usart_common_all.c.o
/Users/Luppy/codal/libraries/codal-libopencm3/stm32/cm/CmSPI.cpp:85:20: warning: 'int codal::_cm::enable_clock(uint32_t)' defined but not used [-Wunused-function]
         static int enable_clock(uint32_t instance)
                    ^~~~~~~~~~~~
/Users/Luppy/codal/libraries/codal-libopencm3/stm32/cm/CmSPI.cpp:69:21: warning: 'codal::_cm::instances' defined but not used [-Wunused-variable]
         static SPI *instances[4];
                     ^~~~~~~~~
[ 54%] Building CXX object libraries/codal-libopencm3/CMakeFiles/stm32.dir/stm32/i2cint/i2cint.cpp.o
[ 54%] Building CXX object libraries/codal-core/CMakeFiles/codal-core.dir/source/drivers/AsciiKeyMap.cpp.o
[ 54%] Building C object libraries/codal-libopencm3/CMakeFiles/libopencm3.dir/lib/libopencm3/lib/stm32/common/usart_common_f124.c.o
[ 55%] Building CXX object libraries/codal-core/CMakeFiles/codal-core.dir/source/drivers/Button.cpp.o
[ 57%] Building CXX object libraries/codal-libopencm3/CMakeFiles/stm32.dir/stm32/logger/logger.cpp.o
[ 57%] Building CXX object libraries/codal-core/CMakeFiles/codal-core.dir/source/drivers/FXOS8700.cpp.o
[ 57%] Building CXX object libraries/codal-libopencm3/CMakeFiles/stm32.dir/stm32/porting/porting.cpp.o
[ 58%] Building C object libraries/codal-libopencm3/CMakeFiles/libopencm3.dir/lib/libopencm3/lib/stm32/f1/adc.c.o
[ 60%] Building C object libraries/codal-libopencm3/CMakeFiles/libopencm3.d[ 65%] Building C object libraries/codal-libopencm3/CMakeFiles/libopencm3.dir/lib/libopencm3/lib/stm32/f1/gpio.c.o
[ 66%] Building C object libraries/codal-libopencm3/CMakeFiles/libopencm3.dir/lib/libopencm3/lib/stm32/f1/i2c.c.o
ir/lib/libopencm3/lib/stm32/f1/dac.c.o
[ 60%] Building CXX object libraries/codal-libopencm3/CMakeFiles/stm32.dir/stm32/simulator/simulator.cpp.o
[ 61%] Building CXX object libraries/codal-core/CMakeFiles/codal-core.dir/source/drivers/FXOS8700Accelerometer.cpp.o
[ 62%] Building C object libraries/codal-libopencm3/CMakeFiles/libopencm3.dir/lib/libopencm3/lib/stm32/f1/dma.c.o
[ 62%] Building CXX object libraries/codal-core/CMakeFiles/codal-core.dir/source/drivers/FXOS8700Magnetometer.cpp.o
[ 63%] Building CXX object libraries/codal-core/CMakeFiles/codal-core.dir/source/drivers/GhostFAT.cpp.o
[ 62%] Building C object libraries/codal-libopencm3/CMakeFiles/libopencm3.dir/lib/libopencm3/lib/stm32/f1/flash.c.o 64%] Building CXX object libraries/codal-libopencm3/CMakeFiles/stm32.dir/stm32/spiint/spiint.cpp.o
[ 67%] Building CXX object libraries/codal-libopencm3/CMakeFiles/stm32.dir/stm32/uartint/uartint.cpp.o
m
[ 68%] Building CXX object libraries/codal-core/CMakeFiles/codal-core.dir/source/drivers/HID.cpp.o
[ 69%] Building CXX object libraries/codal-core/CMakeFiles/codal-core.dir/source/drivers/HIDJoystick.cpp.o
[ 70%] Building C object libraries/codal-libopencm3/CMakeFiles/libopencm3.dir/lib/libopencm3/lib/stm32/f1/pwr.c.o
[ 70%] Building CXX object libraries/codal-core/CMakeFiles/codal-core.dir/source/drivers/HIDKeyboard.cpp.o
[ 70%] Building C object libraries/codal-libopencm3/CMakeFiles/libopencm3.dir/lib/libopencm3/lib/stm32/f1/rcc.c.o
[ 71%] Building C object libraries/codal-libopencm3/CMakeFiles/libopencm3.dir/lib/libopencm3/lib/stm32/f1/rtc.c.o
[ 72%] Building C object libraries/codal-libopencm3/CMakeFiles/libopencm3.dir/lib/libopencm3/lib/stm32/f1/timer.c.o
[ 73%] Building CXX object libraries/codal-core/CMakeFiles/codal-core.dir/source/drivers/HIDMouse.cpp.o
[ 74%] Building CXX object libraries/codal-core/CMakeFiles/codal-core.dir/source/drivers/LEDMatrix.cpp.o
[ 75%] Building CXX object libraries/codal-core/CMakeFiles/codal-core.dir/source/drivers/LIS3DH.cpp.o
[ 75%] Building CXX object libraries/codal-core/CMakeFiles/codal-core.dir/source/drivers/LinearAnalogSensor.cpp.o
[ 76%] Building CXX object libraries/codal-core/CMakeFiles/codal-core.dir/source/drivers/MessageBus.cpp.o
[ 81%] Linking C static library ../../liblibopencm3.a
[ 77%] Building CXX object libraries/codal-core/CMakeFiles/codal-core.dir/source/drivers/MultiButton.cpp.o
[ 78%] Building CXX object libraries/codal-core/CMakeFiles/codal-core.dir/source/drivers/NonLinearAnalogSensor.cpp.o
[ 78%] Building CXX object libraries/codal-core/CMakeFiles/codal-core.dir/source/drivers/ST7735.cpp.o
[ 79%] Building CXX object libraries/codal-core/CMakeFiles/codal-core.dir/source/drivers/StandardSPIFlash.cpp.o
[ 80%] Building CXX object libraries/codal-core/CMakeFiles/codal-core.dir/source/drivers/TouchButton.cpp.o
[ 82%] Building CXX object libraries/codal-core/CMakeFiles/codal-core.dir/source/drivers/TouchSensor.cpp.o
[ 82%] Built target libopencm3
[ 83%] Building CXX object libraries/codal-core/CMakeFiles/codal-core.dir/source/drivers/USBMSC.cpp.o
[ 83%] Building CXX object libraries/codal-core/CMakeFiles/codal-core.dir/source/streams/DataStream.cpp.o
[ 84%] Building CXX object libraries/codal-core/CMakeFiles/codal-core.dir/source/streams/LevelDetectorSPL.cpp.o
[ 85%] Building CXX object libraries/codal-core/CMakeFiles/codal-core.dir/source/streams/LevelDetector.cpp.o
[ 89%] Building CXX object libraries/codal-core/CMakeFiles/codal-core.dir/source/types/Event.cpp.o
[ 89%] Building CXX object libraries/codal-core/CMakeFiles/codal-core.dir/source/types/Image.cpp.o
[ 86%] Building CXX object libraries/codal-core/CMakeFiles/codal-core.dir/source/streams/StreamNormalizer.cpp.o
[ 86%] Building CXX object libraries/codal-core/CMakeFiles/codal-core.dir/source/streams/Synthesizer.cpp.o
[ 87%] Building CXX object libraries/codal-core/CMakeFiles/codal-core.dir/source/types/BitmapFont.cpp.o
[ 88%] Building CXX object libraries/codal-core/CMakeFiles/codal-core.dir/source/types/CoordinateSystem.cpp.o
[ 90%] Building CXX object libraries/codal-core/CMakeFiles/codal-core.dir/source/types/ManagedBuffer.cpp.o
[ 91%] Building CXX object libraries/codal-core/CMakeFiles/codal-core.dir/source/types/ManagedString.cpp.o
[ 92%] Building CXX object libraries/codal-core/CMakeFiles/codal-core.dir/source/types/Matrix4.cpp.o
[ 92%] Building CXX object libraries/codal-core/CMakeFiles/codal-core.dir/source/types/RefCounted.cpp.o
[ 93%] Building CXX object libraries/codal-core/CMakeFiles/codal-core.dir/source/types/RefCountedInit.cpp.o
[ 94%] Linking CXX static library ../../libcodal-core.a
[ 94%] Built target codal-core
[ 95%] Linking CXX static library ../../libstm32.a
[ 95%] Built target stm32
Scanning dependencies of target codal-libopencm3
[ 97%] Building CXX object libraries/codal-libopencm3/CMakeFiles/codal-libopencm3.dir/src/sensor_display.cpp.o
[ 97%] Building ASM object libraries/codal-libopencm3/CMakeFiles/codal-libopencm3.dir/asm/CortexContextSwitch.s.o
[ 97%] Linking CXX static library ../../libcodal-libopencm3.a
[ 97%] Built target codal-libopencm3
Scanning dependencies of target STM32_BLUE_PILL
[ 97%] Building CXX object CMakeFiles/STM32_BLUE_PILL.dir/source/main.cpp.o
[ 98%] Linking CXX executable STM32_BLUE_PILL
Memory region         Used Size  Region Size  %age Used
             rom:       12500 B        64 KB     19.07%
             ram:         824 B        20 KB      4.02%
[ 98%] Built target STM32_BLUE_PILL
Scanning dependencies of target STM32_BLUE_PILL_bin
Scanning dependencies of target STM32_BLUE_PILL_hex
[ 99%] converting to bin file.
[100%] converting to hex file.
[100%] Built target STM32_BLUE_PILL_hex
[100%] Built target STM32_BLUE_PILL_bin

# Luppy at Luppys-MacBook-Pro.local in ~/codal on git:master ✖︎ [13:11:40]
→
```

## PlatformIO Build Log

```sh
> Executing task in folder codal-libopencm3: platformio run <

Processing bluepill_f103c8 (framework: libopencm3; platform: ststm32; board: bluepill_f103c8)
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Verbose mode can be enabled via `-v, --verbose` option
CONFIGURATION: https://docs.platformio.org/page/boards/ststm32/bluepill_f103c8.html
PLATFORM: ST STM32 > BluePill F103C8
HARDWARE: STM32F103C8T6 72MHz 20KB RAM (64KB Flash)
DEBUG: CURRENT(blackmagic) EXTERNAL(blackmagic, jlink, stlink)
Library Dependency Finder -> http://bit.ly/configure-pio-ldf
LDF MODES: FINDER(chain+) COMPATIBILITY(off)
Collected 25 compatible libraries
Scanning dependencies...
Dependency Graph
|-- <boost_iterator>
|   |-- <boost_mpl>
|   |   |-- <boost_preprocessor>
|   |   |-- <boost_static_assert>
|   |   |   |-- <boost_config>
|   |   |-- <boost_config>
|   |   |-- <boost_type_traits>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_static_assert>
|   |   |   |   |-- <boost_config>
|   |   |-- <boost_core>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_assert>
|   |   |-- <boost_utility>
|   |   |   |-- <boost_preprocessor>
|   |   |   |-- <boost_type_traits>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |-- <boost_config>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_core>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_assert>
|   |   |   |-- <boost_static_assert>
|   |   |   |   |-- <boost_config>
|   |   |-- <boost_predef>
|   |-- <boost_preprocessor>
|   |-- <boost_detail>
|   |   |-- <boost_type_traits>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_static_assert>
|   |   |   |   |-- <boost_config>
|   |   |-- <boost_config>
|   |   |-- <boost_preprocessor>
|   |   |-- <boost_assert>
|   |   |-- <boost_static_assert>
|   |   |   |-- <boost_config>
|   |   |-- <boost_core>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_assert>
|   |-- <boost_static_assert>
|   |   |-- <boost_config>
|   |-- <boost_type_traits>
|   |   |-- <boost_config>
|   |   |-- <boost_static_assert>
|   |   |   |-- <boost_config>
|   |-- <boost_config>
|   |-- <boost_core>
|   |   |-- <boost_config>
|   |   |-- <boost_assert>
|   |-- <boost_assert>
|   |-- <boost_utility>
|   |   |-- <boost_preprocessor>
|   |   |-- <boost_type_traits>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_static_assert>
|   |   |   |   |-- <boost_config>
|   |   |-- <boost_config>
|   |   |-- <boost_core>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_assert>
|   |   |-- <boost_static_assert>
|   |   |   |-- <boost_config>
|-- <boost_lockfree>
|   |-- <boost_iterator>
|   |   |-- <boost_mpl>
|   |   |   |-- <boost_preprocessor>
|   |   |   |-- <boost_static_assert>
|   |   |   |   |-- <boost_config>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_type_traits>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |-- <boost_config>
|   |   |   |-- <boost_core>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_assert>
|   |   |   |-- <boost_utility>
|   |   |   |   |-- <boost_preprocessor>
|   |   |   |   |-- <boost_type_traits>
|   |   |   |   |   |-- <boost_config>
|   |   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_core>
|   |   |   |   |   |-- <boost_config>
|   |   |   |   |   |-- <boost_assert>
|   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |-- <boost_config>
|   |   |   |-- <boost_predef>
|   |   |-- <boost_preprocessor>
|   |   |-- <boost_detail>
|   |   |   |-- <boost_type_traits>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |-- <boost_config>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_preprocessor>
|   |   |   |-- <boost_assert>
|   |   |   |-- <boost_static_assert>
|   |   |   |   |-- <boost_config>
|   |   |   |-- <boost_core>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_assert>
|   |   |-- <boost_static_assert>
|   |   |   |-- <boost_config>
|   |   |-- <boost_type_traits>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_static_assert>
|   |   |   |   |-- <boost_config>
|   |   |-- <boost_config>
|   |   |-- <boost_core>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_assert>
|   |   |-- <boost_assert>
|   |   |-- <boost_utility>
|   |   |   |-- <boost_preprocessor>
|   |   |   |-- <boost_type_traits>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |-- <boost_config>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_core>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_assert>
|   |   |   |-- <boost_static_assert>
|   |   |   |   |-- <boost_config>
|   |-- <boost_assert>
|   |-- <boost_static_assert>
|   |   |-- <boost_config>
|   |-- <boost_config>
|   |-- <boost_mpl>
|   |   |-- <boost_preprocessor>
|   |   |-- <boost_static_assert>
|   |   |   |-- <boost_config>
|   |   |-- <boost_config>
|   |   |-- <boost_type_traits>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_static_assert>
|   |   |   |   |-- <boost_config>
|   |   |-- <boost_core>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_assert>
|   |   |-- <boost_utility>
|   |   |   |-- <boost_preprocessor>
|   |   |   |-- <boost_type_traits>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |-- <boost_config>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_core>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_assert>
|   |   |   |-- <boost_static_assert>
|   |   |   |   |-- <boost_config>
|   |   |-- <boost_predef>
|   |-- <boost_parameter>
|   |   |-- <boost_utility>
|   |   |   |-- <boost_preprocessor>
|   |   |   |-- <boost_type_traits>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |-- <boost_config>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_core>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_assert>
|   |   |   |-- <boost_static_assert>
|   |   |   |   |-- <boost_config>
|   |   |-- <boost_preprocessor>
|   |   |-- <boost_mpl>
|   |   |   |-- <boost_preprocessor>
|   |   |   |-- <boost_static_assert>
|   |   |   |   |-- <boost_config>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_type_traits>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |-- <boost_config>
|   |   |   |-- <boost_core>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_assert>
|   |   |   |-- <boost_utility>
|   |   |   |   |-- <boost_preprocessor>
|   |   |   |   |-- <boost_type_traits>
|   |   |   |   |   |-- <boost_config>
|   |   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_core>
|   |   |   |   |   |-- <boost_config>
|   |   |   |   |   |-- <boost_assert>
|   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |-- <boost_config>
|   |   |   |-- <boost_predef>
|   |   |-- <boost_type_traits>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_static_assert>
|   |   |   |   |-- <boost_config>
|   |   |-- <boost_detail>
|   |   |   |-- <boost_type_traits>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |-- <boost_config>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_preprocessor>
|   |   |   |-- <boost_assert>
|   |   |   |-- <boost_static_assert>
|   |   |   |   |-- <boost_config>
|   |   |   |-- <boost_core>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_assert>
|   |   |-- <boost_config>
|   |   |-- <boost_core>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_assert>
|   |-- <boost_type_traits>
|   |   |-- <boost_config>
|   |   |-- <boost_static_assert>
|   |   |   |-- <boost_config>
|   |-- <boost_core>
|   |   |-- <boost_config>
|   |   |-- <boost_assert>
|   |-- <boost_detail>
|   |   |-- <boost_type_traits>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_static_assert>
|   |   |   |   |-- <boost_config>
|   |   |-- <boost_config>
|   |   |-- <boost_preprocessor>
|   |   |-- <boost_assert>
|   |   |-- <boost_static_assert>
|   |   |   |-- <boost_config>
|   |   |-- <boost_core>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_assert>
|   |-- <boost_utility>
|   |   |-- <boost_preprocessor>
|   |   |-- <boost_type_traits>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_static_assert>
|   |   |   |   |-- <boost_config>
|   |   |-- <boost_config>
|   |   |-- <boost_core>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_assert>
|   |   |-- <boost_static_assert>
|   |   |   |-- <boost_config>
|   |-- <boost_predef>
|   |-- <boost_preprocessor>
|-- <logger>
|-- <boost_config>
|-- <bluepill>
|   |-- <cm>
|   |   |-- <logger>
|   |-- <logger>
|   |-- <codal-core>
|   |   |-- <cm>
|   |   |   |-- <logger>
|-- <simulator>
|   |-- <cocoOS> 5.0.4
|   |-- <bluepill>
|   |   |-- <cm>
|   |   |   |-- <logger>
|   |   |-- <logger>
|   |   |-- <codal-core>
|   |   |   |-- <cm>
|   |   |   |   |-- <logger>
|   |-- <logger>
|   |-- <spiint>
|   |   |-- <cocoOS> 5.0.4
|   |   |-- <bluepill>
|   |   |   |-- <cm>
|   |   |   |   |-- <logger>
|   |   |   |-- <logger>
|   |   |   |-- <codal-core>
|   |   |   |   |-- <cm>
|   |   |   |   |   |-- <logger>
|   |   |-- <logger>
|-- <boost_parameter>
|   |-- <boost_utility>
|   |   |-- <boost_preprocessor>
|   |   |-- <boost_type_traits>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_static_assert>
|   |   |   |   |-- <boost_config>
|   |   |-- <boost_config>
|   |   |-- <boost_core>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_assert>
|   |   |-- <boost_static_assert>
|   |   |   |-- <boost_config>
|   |-- <boost_preprocessor>
|   |-- <boost_mpl>
|   |   |-- <boost_preprocessor>
|   |   |-- <boost_static_assert>
|   |   |   |-- <boost_config>
|   |   |-- <boost_config>
|   |   |-- <boost_type_traits>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_static_assert>
|   |   |   |   |-- <boost_config>
|   |   |-- <boost_core>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_assert>
|   |   |-- <boost_utility>
|   |   |   |-- <boost_preprocessor>
|   |   |   |-- <boost_type_traits>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |-- <boost_config>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_core>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_assert>
|   |   |   |-- <boost_static_assert>
|   |   |   |   |-- <boost_config>
|   |   |-- <boost_predef>
|   |-- <boost_type_traits>
|   |   |-- <boost_config>
|   |   |-- <boost_static_assert>
|   |   |   |-- <boost_config>
|   |-- <boost_detail>
|   |   |-- <boost_type_traits>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_static_assert>
|   |   |   |   |-- <boost_config>
|   |   |-- <boost_config>
|   |   |-- <boost_preprocessor>
|   |   |-- <boost_assert>
|   |   |-- <boost_static_assert>
|   |   |   |-- <boost_config>
|   |   |-- <boost_core>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_assert>
|   |-- <boost_config>
|   |-- <boost_core>
|   |   |-- <boost_config>
|   |   |-- <boost_assert>
|-- <boost_preprocessor>
|-- <uartint>
|   |-- <boost_lockfree>
|   |   |-- <boost_iterator>
|   |   |   |-- <boost_mpl>
|   |   |   |   |-- <boost_preprocessor>
|   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_type_traits>
|   |   |   |   |   |-- <boost_config>
|   |   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_core>
|   |   |   |   |   |-- <boost_config>
|   |   |   |   |   |-- <boost_assert>
|   |   |   |   |-- <boost_utility>
|   |   |   |   |   |-- <boost_preprocessor>
|   |   |   |   |   |-- <boost_type_traits>
|   |   |   |   |   |   |-- <boost_config>
|   |   |   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |   |   |-- <boost_config>
|   |   |   |   |   |-- <boost_config>
|   |   |   |   |   |-- <boost_core>
|   |   |   |   |   |   |-- <boost_config>
|   |   |   |   |   |   |-- <boost_assert>
|   |   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_predef>
|   |   |   |-- <boost_preprocessor>
|   |   |   |-- <boost_detail>
|   |   |   |   |-- <boost_type_traits>
|   |   |   |   |   |-- <boost_config>
|   |   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_preprocessor>
|   |   |   |   |-- <boost_assert>
|   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_core>
|   |   |   |   |   |-- <boost_config>
|   |   |   |   |   |-- <boost_assert>
|   |   |   |-- <boost_static_assert>
|   |   |   |   |-- <boost_config>
|   |   |   |-- <boost_type_traits>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |-- <boost_config>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_core>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_assert>
|   |   |   |-- <boost_assert>
|   |   |   |-- <boost_utility>
|   |   |   |   |-- <boost_preprocessor>
|   |   |   |   |-- <boost_type_traits>
|   |   |   |   |   |-- <boost_config>
|   |   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_core>
|   |   |   |   |   |-- <boost_config>
|   |   |   |   |   |-- <boost_assert>
|   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |-- <boost_config>
|   |   |-- <boost_assert>
|   |   |-- <boost_static_assert>
|   |   |   |-- <boost_config>
|   |   |-- <boost_config>
|   |   |-- <boost_mpl>
|   |   |   |-- <boost_preprocessor>
|   |   |   |-- <boost_static_assert>
|   |   |   |   |-- <boost_config>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_type_traits>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |-- <boost_config>
|   |   |   |-- <boost_core>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_assert>
|   |   |   |-- <boost_utility>
|   |   |   |   |-- <boost_preprocessor>
|   |   |   |   |-- <boost_type_traits>
|   |   |   |   |   |-- <boost_config>
|   |   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_core>
|   |   |   |   |   |-- <boost_config>
|   |   |   |   |   |-- <boost_assert>
|   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |-- <boost_config>
|   |   |   |-- <boost_predef>
|   |   |-- <boost_parameter>
|   |   |   |-- <boost_utility>
|   |   |   |   |-- <boost_preprocessor>
|   |   |   |   |-- <boost_type_traits>
|   |   |   |   |   |-- <boost_config>
|   |   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_core>
|   |   |   |   |   |-- <boost_config>
|   |   |   |   |   |-- <boost_assert>
|   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |-- <boost_config>
|   |   |   |-- <boost_preprocessor>
|   |   |   |-- <boost_mpl>
|   |   |   |   |-- <boost_preprocessor>
|   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_type_traits>
|   |   |   |   |   |-- <boost_config>
|   |   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_core>
|   |   |   |   |   |-- <boost_config>
|   |   |   |   |   |-- <boost_assert>
|   |   |   |   |-- <boost_utility>
|   |   |   |   |   |-- <boost_preprocessor>
|   |   |   |   |   |-- <boost_type_traits>
|   |   |   |   |   |   |-- <boost_config>
|   |   |   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |   |   |-- <boost_config>
|   |   |   |   |   |-- <boost_config>
|   |   |   |   |   |-- <boost_core>
|   |   |   |   |   |   |-- <boost_config>
|   |   |   |   |   |   |-- <boost_assert>
|   |   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_predef>
|   |   |   |-- <boost_type_traits>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |-- <boost_config>
|   |   |   |-- <boost_detail>
|   |   |   |   |-- <boost_type_traits>
|   |   |   |   |   |-- <boost_config>
|   |   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_preprocessor>
|   |   |   |   |-- <boost_assert>
|   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_core>
|   |   |   |   |   |-- <boost_config>
|   |   |   |   |   |-- <boost_assert>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_core>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_assert>
|   |   |-- <boost_type_traits>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_static_assert>
|   |   |   |   |-- <boost_config>
|   |   |-- <boost_core>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_assert>
|   |   |-- <boost_detail>
|   |   |   |-- <boost_type_traits>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |-- <boost_config>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_preprocessor>
|   |   |   |-- <boost_assert>
|   |   |   |-- <boost_static_assert>
|   |   |   |   |-- <boost_config>
|   |   |   |-- <boost_core>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_assert>
|   |   |-- <boost_utility>
|   |   |   |-- <boost_preprocessor>
|   |   |   |-- <boost_type_traits>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |-- <boost_config>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_core>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_assert>
|   |   |   |-- <boost_static_assert>
|   |   |   |   |-- <boost_config>
|   |   |-- <boost_predef>
|   |   |-- <boost_preprocessor>
|   |-- <logger>
|   |-- <bluepill>
|   |   |-- <cm>
|   |   |   |-- <logger>
|   |   |-- <logger>
|   |   |-- <codal-core>
|   |   |   |-- <cm>
|   |   |   |   |-- <logger>
|-- <boost_predef>
|-- <boost_type_traits>
|   |-- <boost_config>
|   |-- <boost_static_assert>
|   |   |-- <boost_config>
|-- <codal-core>
|   |-- <cm>
|   |   |-- <logger>
|-- <boost_core>
|   |-- <boost_config>
|   |-- <boost_assert>
|-- <cocoOS> 5.0.4
|-- <i2cint>
|   |-- <bluepill>
|   |   |-- <cm>
|   |   |   |-- <logger>
|   |   |-- <logger>
|   |   |-- <codal-core>
|   |   |   |-- <cm>
|   |   |   |   |-- <logger>
|   |-- <logger>
|-- <spiint>
|   |-- <cocoOS> 5.0.4
|   |-- <bluepill>
|   |   |-- <cm>
|   |   |   |-- <logger>
|   |   |-- <logger>
|   |   |-- <codal-core>
|   |   |   |-- <cm>
|   |   |   |   |-- <logger>
|   |-- <logger>
|-- <boost_detail>
|   |-- <boost_type_traits>
|   |   |-- <boost_config>
|   |   |-- <boost_static_assert>
|   |   |   |-- <boost_config>
|   |-- <boost_config>
|   |-- <boost_preprocessor>
|   |-- <boost_assert>
|   |-- <boost_static_assert>
|   |   |-- <boost_config>
|   |-- <boost_core>
|   |   |-- <boost_config>
|   |   |-- <boost_assert>
|-- <porting>
|   |-- <cocoOS> 5.0.4
|   |-- <i2cint>
|   |   |-- <bluepill>
|   |   |   |-- <cm>
|   |   |   |   |-- <logger>
|   |   |   |-- <logger>
|   |   |   |-- <codal-core>
|   |   |   |   |-- <cm>
|   |   |   |   |   |-- <logger>
|   |   |-- <logger>
|   |-- <spiint>
|   |   |-- <cocoOS> 5.0.4
|   |   |-- <bluepill>
|   |   |   |-- <cm>
|   |   |   |   |-- <logger>
|   |   |   |-- <logger>
|   |   |   |-- <codal-core>
|   |   |   |   |-- <cm>
|   |   |   |   |   |-- <logger>
|   |   |-- <logger>
|   |-- <bluepill>
|   |   |-- <cm>
|   |   |   |-- <logger>
|   |   |-- <logger>
|   |   |-- <codal-core>
|   |   |   |-- <cm>
|   |   |   |   |-- <logger>
|-- <BME280> 2.3.0
|   |-- <porting>
|   |   |-- <cocoOS> 5.0.4
|   |   |-- <i2cint>
|   |   |   |-- <bluepill>
|   |   |   |   |-- <cm>
|   |   |   |   |   |-- <logger>
|   |   |   |   |-- <logger>
|   |   |   |   |-- <codal-core>
|   |   |   |   |   |-- <cm>
|   |   |   |   |   |   |-- <logger>
|   |   |   |-- <logger>
|   |   |-- <spiint>
|   |   |   |-- <cocoOS> 5.0.4
|   |   |   |-- <bluepill>
|   |   |   |   |-- <cm>
|   |   |   |   |   |-- <logger>
|   |   |   |   |-- <logger>
|   |   |   |   |-- <codal-core>
|   |   |   |   |   |-- <cm>
|   |   |   |   |   |   |-- <logger>
|   |   |   |-- <logger>
|   |   |-- <bluepill>
|   |   |   |-- <cm>
|   |   |   |   |-- <logger>
|   |   |   |-- <logger>
|   |   |   |-- <codal-core>
|   |   |   |   |-- <cm>
|   |   |   |   |   |-- <logger>
|   |-- <cocoOS> 5.0.4
|   |-- <spiint>
|   |   |-- <cocoOS> 5.0.4
|   |   |-- <bluepill>
|   |   |   |-- <cm>
|   |   |   |   |-- <logger>
|   |   |   |-- <logger>
|   |   |   |-- <codal-core>
|   |   |   |   |-- <cm>
|   |   |   |   |   |-- <logger>
|   |   |-- <logger>
|   |-- <i2cint>
|   |   |-- <bluepill>
|   |   |   |-- <cm>
|   |   |   |   |-- <logger>
|   |   |   |-- <logger>
|   |   |   |-- <codal-core>
|   |   |   |   |-- <cm>
|   |   |   |   |   |-- <logger>
|   |   |-- <logger>
|   |-- <codal-core>
|   |   |-- <cm>
|   |   |   |-- <logger>
|   |-- <logger>
|   |-- <cm>
|   |   |-- <logger>
|-- <boost_assert>
|-- <boost_mpl>
|   |-- <boost_preprocessor>
|   |-- <boost_static_assert>
|   |   |-- <boost_config>
|   |-- <boost_config>
|   |-- <boost_type_traits>
|   |   |-- <boost_config>
|   |   |-- <boost_static_assert>
|   |   |   |-- <boost_config>
|   |-- <boost_core>
|   |   |-- <boost_config>
|   |   |-- <boost_assert>
|   |-- <boost_utility>
|   |   |-- <boost_preprocessor>
|   |   |-- <boost_type_traits>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_static_assert>
|   |   |   |   |-- <boost_config>
|   |   |-- <boost_config>
|   |   |-- <boost_core>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_assert>
|   |   |-- <boost_static_assert>
|   |   |   |-- <boost_config>
|   |-- <boost_predef>
|-- <boost_static_assert>
|   |-- <boost_config>
|-- <cm>
|   |-- <logger>
|-- <boost_utility>
|   |-- <boost_preprocessor>
|   |-- <boost_type_traits>
|   |   |-- <boost_config>
|   |   |-- <boost_static_assert>
|   |   |   |-- <boost_config>
|   |-- <boost_config>
|   |-- <boost_core>
|   |   |-- <boost_config>
|   |   |-- <boost_assert>
|   |-- <boost_static_assert>
|   |   |-- <boost_config>
Compiling .pioenvs/bluepill_f103c8/src/aggregate.o
Compiling .pioenvs/bluepill_f103c8/src/alt_sensor.o
Compiling .pioenvs/bluepill_f103c8/src/bme280.o
Compiling .pioenvs/bluepill_f103c8/src/display.o
Compiling .pioenvs/bluepill_f103c8/src/downlink.o
Compiling .pioenvs/bluepill_f103c8/src/gyro_sensor.o
Compiling .pioenvs/bluepill_f103c8/src/humid_sensor.o
Compiling .pioenvs/bluepill_f103c8/src/main.o
Compiling .pioenvs/bluepill_f103c8/src/sensor.o
Compiling .pioenvs/bluepill_f103c8/src/temp_event_sensor.o
Compiling .pioenvs/bluepill_f103c8/src/temp_sensor.o
src/sensor.cpp:213:13: warning: 'bool is_valid_event_sensor(Sensor*)' defined but not used [-Wunused-function]
static bool is_valid_event_sensor(Sensor *sensor) {
^~~~~~~~~~~~~~~~~~~~~
Compiling .pioenvs/bluepill_f103c8/src/uart.o
Compiling .pioenvs/bluepill_f103c8/src/wisol.o
Compiling .pioenvs/bluepill_f103c8/src/wstring.o
Archiving .pioenvs/bluepill_f103c8/libd23/libboost_preprocessor.a
Archiving .pioenvs/bluepill_f103c8/lib23b/libboost_config.a
Indexing .pioenvs/bluepill_f103c8/libd23/libboost_preprocessor.a
Indexing .pioenvs/bluepill_f103c8/lib23b/libboost_config.a
Archiving .pioenvs/bluepill_f103c8/lib186/libboost_static_assert.a
Archiving .pioenvs/bluepill_f103c8/lib047/libboost_type_traits.a
Archiving .pioenvs/bluepill_f103c8/libf3d/libboost_assert.a
Indexing .pioenvs/bluepill_f103c8/lib186/libboost_static_assert.a
Archiving .pioenvs/bluepill_f103c8/lib3c5/libboost_core.a
Indexing .pioenvs/bluepill_f103c8/lib047/libboost_type_traits.a
Archiving .pioenvs/bluepill_f103c8/libd30/libboost_utility.a
Indexing .pioenvs/bluepill_f103c8/libf3d/libboost_assert.a
Indexing .pioenvs/bluepill_f103c8/lib3c5/libboost_core.a
Indexing .pioenvs/bluepill_f103c8/libd30/libboost_utility.a
Archiving .pioenvs/bluepill_f103c8/lib64e/libboost_predef.a
Archiving .pioenvs/bluepill_f103c8/libe2b/libboost_mpl.a
Archiving .pioenvs/bluepill_f103c8/lib95e/libboost_detail.a
Archiving .pioenvs/bluepill_f103c8/lib609/libboost_iterator.a
Indexing .pioenvs/bluepill_f103c8/lib64e/libboost_predef.a
Archiving .pioenvs/bluepill_f103c8/libcea/libboost_parameter.a
Indexing .pioenvs/bluepill_f103c8/libe2b/libboost_mpl.a
Indexing .pioenvs/bluepill_f103c8/lib95e/libboost_detail.a
Indexing .pioenvs/bluepill_f103c8/lib609/libboost_iterator.a
Indexing .pioenvs/bluepill_f103c8/libcea/libboost_parameter.a
Archiving .pioenvs/bluepill_f103c8/lib7b0/libboost_lockfree.a
Compiling .pioenvs/bluepill_f103c8/libd9b/logger/logger.o
Indexing .pioenvs/bluepill_f103c8/lib7b0/libboost_lockfree.a
Compiling .pioenvs/bluepill_f103c8/lib739/cm/CmI2C.o
Compiling .pioenvs/bluepill_f103c8/lib739/cm/CmPWM.o
Compiling .pioenvs/bluepill_f103c8/lib739/cm/CmPin.o
Compiling .pioenvs/bluepill_f103c8/lib739/cm/CmSPI.o
Archiving .pioenvs/bluepill_f103c8/libd9b/liblogger.a
Compiling .pioenvs/bluepill_f103c8/lib739/cm/CmSingleWireSerial.o
Indexing .pioenvs/bluepill_f103c8/libd9b/liblogger.a
stm32/cm/CmSPI.cpp:85:20: warning: 'int codal::_cm::enable_clock(uint32_t)' defined but not used [-Wunused-function]
static int enable_clock(uint32_t instance)
^~~~~~~~~~~~
stm32/cm/CmSPI.cpp:69:21: warning: 'codal::_cm::instances' defined but not used [-Wunused-variable]
static SPI *instances[4];
^~~~~~~~~
Compiling .pioenvs/bluepill_f103c8/lib739/cm/CmTimer.o
Compiling .pioenvs/bluepill_f103c8/lib739/cm/CmUSB.o
Compiling .pioenvs/bluepill_f103c8/lib739/cm/DynamicPwm.o
Compiling .pioenvs/bluepill_f103c8/lib739/cm/codal_target_hal.o
Compiling .pioenvs/bluepill_f103c8/lib739/cm/dma.o
Compiling .pioenvs/bluepill_f103c8/lib739/cm/irq.o
Compiling .pioenvs/bluepill_f103c8/lib739/cm/pinmap.o
Compiling .pioenvs/bluepill_f103c8/lib739/cm/pinmap_common.o
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
Compiling .pioenvs/bluepill_f103c8/lib739/cm/pwmout_api.o
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Compiling .pioenvs/bluepill_f103c8/lib739/cm/pwmout_device.o
stm32/cm/irq.c: In function 'SysTick_Handler':
stm32/cm/irq.c:15:5: warning: implicit declaration of function 'HAL_IncTick' [-Wimplicit-function-declaration]
HAL_IncTick();
^~~~~~~~~~~
In file included from stm32/cm/pinmap.c:32:0:
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/core/CodalCompat.o
stm32/cm/pin_device.h: In function 'stm_pin_SetAFPin':
stm32/cm/pin_device.h:68:9: warning: implicit declaration of function 'LL_GPIO_SetAFPin_8_15' [-Wimplicit-function-declaration]
LL_GPIO_SetAFPin_8_15(gpio, ll_pin, afnum);
^~~~~~~~~~~~~~~~~~~~~
Archiving .pioenvs/bluepill_f103c8/lib739/libcm.a
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/core/CodalComponent.o
stm32/cm/pin_device.h:70:9: warning: implicit declaration of function 'LL_GPIO_SetAFPin_0_7' [-Wimplicit-function-declaration]
LL_GPIO_SetAFPin_0_7(gpio, ll_pin, afnum);
Indexing .pioenvs/bluepill_f103c8/lib739/libcm.a
^~~~~~~~~~~~~~~~~~~~
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/core/CodalDevice.o
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/core/CodalDmesg.o
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/core/CodalFiber.o
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/core/CodalHeapAllocator.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/core/CodalListener.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/core/CodalUtil.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/core/MemberFunctionCallback.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/driver-models/AbstractButton.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/driver-models/Accelerometer.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/driver-models/CodalUSB.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/driver-models/Compass.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/driver-models/Display.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/driver-models/Gyroscope.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/driver-models/I2C.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/driver-models/SPI.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/driver-models/Sensor.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/driver-models/Timer.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/drivers/AnalogSensor.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/drivers/AnimatedDisplay.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/drivers/AsciiKeyMap.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/drivers/Button.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/drivers/FXOS8700.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/drivers/FXOS8700Accelerometer.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/drivers/FXOS8700Magnetometer.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/drivers/GhostFAT.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/drivers/HID.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/drivers/HIDJoystick.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/drivers/HIDKeyboard.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/drivers/HIDMouse.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/drivers/LEDMatrix.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/drivers/LIS3DH.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/drivers/LinearAnalogSensor.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/drivers/MessageBus.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/drivers/MultiButton.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/drivers/NonLinearAnalogSensor.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/drivers/ST7735.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/drivers/StandardSPIFlash.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/drivers/TouchButton.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/drivers/TouchSensor.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/drivers/USBMSC.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/streams/DataStream.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/streams/LevelDetector.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/streams/LevelDetectorSPL.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/streams/StreamNormalizer.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/streams/Synthesizer.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/types/BitmapFont.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/types/CoordinateSystem.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/types/Event.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/types/Image.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/types/ManagedBuffer.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/types/ManagedString.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/types/Matrix4.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/types/RefCounted.o
Compiling .pioenvs/bluepill_f103c8/lib3e7/codal-core/source/types/RefCountedInit.o
Compiling .pioenvs/bluepill_f103c8/lib271/bluepill/STM32BluePill.o
Compiling .pioenvs/bluepill_f103c8/lib271/bluepill/STM32BluePillIO.o
Compiling .pioenvs/bluepill_f103c8/lib271/bluepill/STM32BluePillPin.o
Compiling .pioenvs/bluepill_f103c8/lib271/bluepill/bluepill.o
Compiling .pioenvs/bluepill_f103c8/lib271/bluepill/led.o
Compiling .pioenvs/bluepill_f103c8/lib271/bluepill/timer.o
Compiling .pioenvs/bluepill_f103c8/libea7/cocoOS_5.0.3/os_assert.o
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Archiving .pioenvs/bluepill_f103c8/lib3e7/libcodal-core.a
Compiling .pioenvs/bluepill_f103c8/libea7/cocoOS_5.0.3/os_cbk.o
Indexing .pioenvs/bluepill_f103c8/lib3e7/libcodal-core.a
Compiling .pioenvs/bluepill_f103c8/libea7/cocoOS_5.0.3/os_event.o
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Compiling .pioenvs/bluepill_f103c8/libea7/cocoOS_5.0.3/os_kernel.o
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Compiling .pioenvs/bluepill_f103c8/libea7/cocoOS_5.0.3/os_msgqueue.o
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Compiling .pioenvs/bluepill_f103c8/libea7/cocoOS_5.0.3/os_sem.o
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
lib/cocoOS_5.0.3/src/os_msgqueue.c: In function 'queue_push':
lib/cocoOS_5.0.3/src/os_msgqueue.c:160:28: warning: cast from pointer to integer of different size [-Wpointer-to-int-cast]
uint8_t *dst = (uint8_t*)((Mem_t)queue->list + head * msgSz);
Archiving .pioenvs/bluepill_f103c8/lib271/libbluepill.a
^
lib/cocoOS_5.0.3/src/os_msgqueue.c:160:17: warning: cast to pointer from integer of different size [-Wint-to-pointer-cast]
Indexing .pioenvs/bluepill_f103c8/lib271/libbluepill.a
uint8_t *dst = (uint8_t*)((Mem_t)queue->list + head * msgSz);
Compiling .pioenvs/bluepill_f103c8/libea7/cocoOS_5.0.3/os_task.o
^
lib/cocoOS_5.0.3/src/os_msgqueue.c: In function 'os_msg_receive':
lib/cocoOS_5.0.3/src/os_msgqueue.c:199:27: warning: cast from pointer to integer of different size [-Wpointer-to-int-cast]
src = (uint8_t*)( (Mem_t)q->list + tail * msgSz );
^
lib/cocoOS_5.0.3/src/os_msgqueue.c:199:15: warning: cast to pointer from integer of different size [-Wint-to-pointer-cast]
src = (uint8_t*)( (Mem_t)q->list + tail * msgSz );
^
lib/cocoOS_5.0.3/src/os_msgqueue.c:221:31: warning: cast from pointer to integer of different size [-Wpointer-to-int-cast]
Compiling .pioenvs/bluepill_f103c8/lib91e/spiint/spiint.o
dst = (uint8_t*)( (Mem_t)q->list + q->head * msgSz );
^
lib/cocoOS_5.0.3/src/os_msgqueue.c:221:19: warning: cast to pointer from integer of different size [-Wint-to-pointer-cast]
dst = (uint8_t*)( (Mem_t)q->list + q->head * msgSz );
^
lib/cocoOS_5.0.3/src/os_msgqueue.c: In function 'os_msgQ_tick':
lib/cocoOS_5.0.3/src/os_msgqueue.c:255:26: warning: cast from pointer to integer of different size [-Wpointer-to-int-cast]
pMsg = (Msg_t*)( (Mem_t)q->list + nextMessage * msgSz );
^
lib/cocoOS_5.0.3/src/os_msgqueue.c:255:16: warning: cast to pointer from integer of different size [-Wint-to-pointer-cast]
pMsg = (Msg_t*)( (Mem_t)q->list + nextMessage * msgSz );
^
lib/cocoOS_5.0.3/src/os_msgqueue.c: In function 'MsgQAllDelayed':
lib/cocoOS_5.0.3/src/os_msgqueue.c:284:26: warning: cast from pointer to integer of different size [-Wpointer-to-int-cast]
Compiling .pioenvs/bluepill_f103c8/libc3f/simulator/simulator.o
pMsg = (Msg_t*)( (Mem_t)q->list + (nextMessage * msgSz) );
^
lib/cocoOS_5.0.3/src/os_msgqueue.c:284:16: warning: cast to pointer from integer of different size [-Wint-to-pointer-cast]
pMsg = (Msg_t*)( (Mem_t)q->list + (nextMessage * msgSz) );
^
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Compiling .pioenvs/bluepill_f103c8/lib33e/uartint/uartint.o
Compiling .pioenvs/bluepill_f103c8/libea2/i2cint/i2cint.o
Archiving .pioenvs/bluepill_f103c8/lib91e/libspiint.a
Archiving .pioenvs/bluepill_f103c8/libc3f/libsimulator.a
Archiving .pioenvs/bluepill_f103c8/libea7/libcocoOS_5.0.3.a
Indexing .pioenvs/bluepill_f103c8/lib91e/libspiint.a
Indexing .pioenvs/bluepill_f103c8/libc3f/libsimulator.a
Indexing .pioenvs/bluepill_f103c8/libea7/libcocoOS_5.0.3.a
Compiling .pioenvs/bluepill_f103c8/lib9ba/porting/porting.o
Compiling .pioenvs/bluepill_f103c8/libdc9/BME280/BME280.o
Compiling .pioenvs/bluepill_f103c8/libdc9/BME280/BME280I2C.o
Archiving .pioenvs/bluepill_f103c8/libea2/libi2cint.a
Compiling .pioenvs/bluepill_f103c8/libdc9/BME280/BME280Spi.o
Indexing .pioenvs/bluepill_f103c8/libea2/libi2cint.a
lib/BME280/src/BME280.cpp: In member function 'bool BME280::WriteSettings()':
lib/BME280/src/BME280.cpp:98:1: warning: no return statement in function returning non-void [-Wreturn-type]
}
^
Compiling .pioenvs/bluepill_f103c8/libdc9/BME280/EnvironmentCalculations.o
Compiling .pioenvs/bluepill_f103c8/FrameworkLibOpenCM3/lib/stm32/f1/adc.o
Archiving .pioenvs/bluepill_f103c8/lib9ba/libporting.a
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Compiling .pioenvs/bluepill_f103c8/FrameworkLibOpenCM3/lib/stm32/common/adc_common_v1.o
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Indexing .pioenvs/bluepill_f103c8/lib9ba/libporting.a
Compiling .pioenvs/bluepill_f103c8/FrameworkLibOpenCM3/lib/stm32/can.o
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Archiving .pioenvs/bluepill_f103c8/libdc9/libBME280.a
Compiling .pioenvs/bluepill_f103c8/FrameworkLibOpenCM3/lib/stm32/desig.o
Indexing .pioenvs/bluepill_f103c8/libdc9/libBME280.a
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Compiling .pioenvs/bluepill_f103c8/FrameworkLibOpenCM3/lib/stm32/f1/ethernet.o
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Compiling .pioenvs/bluepill_f103c8/FrameworkLibOpenCM3/lib/stm32/f1/flash.o
Compiling .pioenvs/bluepill_f103c8/FrameworkLibOpenCM3/lib/stm32/f1/gpio.o
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
Compiling .pioenvs/bluepill_f103c8/FrameworkLibOpenCM3/lib/stm32/f1/rcc.o
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Compiling .pioenvs/bluepill_f103c8/FrameworkLibOpenCM3/lib/stm32/f1/rtc.o
Compiling .pioenvs/bluepill_f103c8/FrameworkLibOpenCM3/lib/stm32/f1/timer.o
Compiling .pioenvs/bluepill_f103c8/FrameworkLibOpenCM3/lib/ethernet/mac.o
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Compiling .pioenvs/bluepill_f103c8/FrameworkLibOpenCM3/lib/ethernet/mac_stm32fxx7.o
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Compiling .pioenvs/bluepill_f103c8/FrameworkLibOpenCM3/lib/ethernet/phy.o
Compiling .pioenvs/bluepill_f103c8/FrameworkLibOpenCM3/lib/ethernet/phy_ksz8051mll.o
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Compiling .pioenvs/bluepill_f103c8/FrameworkLibOpenCM3/lib/stm32/common/crc_common_all.o
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Compiling .pioenvs/bluepill_f103c8/FrameworkLibOpenCM3/lib/stm32/common/dac_common_all.o
Compiling .pioenvs/bluepill_f103c8/FrameworkLibOpenCM3/lib/stm32/common/dma_common_l1f013.o
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
Compiling .pioenvs/bluepill_f103c8/FrameworkLibOpenCM3/lib/stm32/common/gpio_common_all.o
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
Compiling .pioenvs/bluepill_f103c8/FrameworkLibOpenCM3/lib/stm32/common/i2c_common_all.o
Compiling .pioenvs/bluepill_f103c8/FrameworkLibOpenCM3/lib/stm32/common/iwdg_common_all.o
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Compiling .pioenvs/bluepill_f103c8/FrameworkLibOpenCM3/lib/stm32/common/pwr_common_all.o
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Compiling .pioenvs/bluepill_f103c8/FrameworkLibOpenCM3/lib/stm32/common/spi_common_all.o
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Compiling .pioenvs/bluepill_f103c8/FrameworkLibOpenCM3/lib/stm32/common/spi_common_l1f124.o
Compiling .pioenvs/bluepill_f103c8/FrameworkLibOpenCM3/lib/stm32/common/timer_common_all.o
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Compiling .pioenvs/bluepill_f103c8/FrameworkLibOpenCM3/lib/stm32/common/usart_common_all.o
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Compiling .pioenvs/bluepill_f103c8/FrameworkLibOpenCM3/lib/stm32/common/usart_common_f124.o
Compiling .pioenvs/bluepill_f103c8/FrameworkLibOpenCM3/lib/stm32/common/rcc_common_all.o
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
Compiling .pioenvs/bluepill_f103c8/FrameworkLibOpenCM3/lib/stm32/common/exti_common_all.o
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-staticCompiling .pioenvs/bluepill_f103c8/FrameworkLibOpenCM3/lib/stm32/common/flash_common_f01.o
s' is valid for C++/ObjC++ but not for C
Compiling .pioenvs/bluepill_f103c8/FrameworkLibOpenCM3/lib/usb/usb.o
Compiling .pioenvs/bluepill_f103c8/FrameworkLibOpenCM3/lib/usb/usb_control.o
Compiling .pioenvs/bluepill_f103c8/FrameworkLibOpenCM3/lib/usb/usb_standard.o
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Compiling .pioenvs/bluepill_f103c8/FrameworkLibOpenCM3/lib/usb/usb_msc.o
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Archiving .pioenvs/bluepill_f103c8/lib33e/libuartint.a
Indexing .pioenvs/bluepill_f103c8/lib33e/libuartint.a
Compiling .pioenvs/bluepill_f103c8/FrameworkLibOpenCM3/lib/usb/usb_fx07_common.o
Compiling .pioenvs/bluepill_f103c8/FrameworkLibOpenCM3/lib/usb/usb_f107.o
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Compiling .pioenvs/bluepill_f103c8/FrameworkLibOpenCM3/lib/stm32/common/st_usbfs_core.o
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Compiling .pioenvs/bluepill_f103c8/FrameworkLibOpenCM3/lib/stm32/st_usbfs_v1.o
Compiling .pioenvs/bluepill_f103c8/FrameworkLibOpenCM3/lib/cm3/vector.o
Compiling .pioenvs/bluepill_f103c8/FrameworkLibOpenCM3/lib/cm3/systick.o
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Compiling .pioenvs/bluepill_f103c8/FrameworkLibOpenCM3/lib/cm3/scb.o
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
Compiling .pioenvs/bluepill_f103c8/FrameworkLibOpenCM3/lib/cm3/nvic.o
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
Compiling .pioenvs/bluepill_f103c8/FrameworkLibOpenCM3/lib/cm3/assert.o
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Compiling .pioenvs/bluepill_f103c8/FrameworkLibOpenCM3/lib/cm3/sync.o
Compiling .pioenvs/bluepill_f103c8/FrameworkLibOpenCM3/lib/cm3/dwt.o
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Archiving .pioenvs/bluepill_f103c8/libFrameworkLibOpenCM3.a
Indexing .pioenvs/bluepill_f103c8/libFrameworkLibOpenCM3.a
Linking .pioenvs/bluepill_f103c8/firmware.elf
Memory region         Used Size  Region Size  %age Used
rom:       30612 B        64 KB     46.71%
ram:        4148 B        20 KB     20.25%
Checking size .pioenvs/bluepill_f103c8/firmware.elf
Building .pioenvs/bluepill_f103c8/firmware.bin
Memory Usage -> http://bit.ly/pio-memory-usage
DATA:    [==        ]  20.3% (used 4148 bytes from 20480 bytes)
PROGRAM: [=====     ]  46.7% (used 30580 bytes from 65536 bytes)
============================================================================================================================ [SUCCESS] Took 40.58 seconds ============================================================================================================================

Terminal will be reused by tasks, press any key to close it.
```