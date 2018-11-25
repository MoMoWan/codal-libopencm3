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

## Building With Codal

- Create a `codal.json` file. See below for the content.
- In the root of this repository type `python build.py` the `-c` option cleans before building.
- The bin file `STM32_BLUE_PILL.bin` and hex file `STM32_BLUE_PILL.hex` will be placed at the location specified by `codal.json`, by default this is the root.
- The ELF executable file is generated in `build/STM32_BLUE_PILL`. To dump the file: `objdump -t build/STM32_BLUE_PILL`
- For troubleshooting, the linker output map is generated in `build/STM32_BLUE_PILL.map`. This is configured in `target.json` and `target-locked.json` in `codal-libopencm3`. 

## Building With Visual Studio Code and PlatformIO

The project may also be build in Visual Studio Code with PlatformIO extension for quick testing.

Launch Visual Studio Code and open the workspace file `workspace.code-workspace`

Steps to download this repository:

```cmd
git clone https://github.com/lupyuen/codal-libopencm3
cd codal-libopencm3

cd lib
mkdir codal-core
cd codal-core
git clone https://github.com/lancaster-university/codal-core
cd ..
cd ..

cd lib
mkdir libopencm3
cd libopencm3
git clone https://github.com/libopencm3/libopencm3
cd ..
cd ..
```

In Visual Studio Code, create the file `codal-libopencm3/lib/codal-core/library.json` with the following content:

```json
{
    "name": "codal-core",
    "frameworks": "libopencm3",
    "repository": {
        "type": "git",
        "url": "https://github.com/lancaster-university/codal-core"
    },
    "description": "Stub for compiling codal-core under PlatformIO",
    "build": {        
        "flags": [
            "-Iinc -Iinc/core -Iinc/driver-models -Iinc/drivers -Iinc/streams -Iinc/types -Iinc/JACDAC"
        ],
        "srcFilter": [
            "+<inc/*>",
            "+<source/*>"
        ]
    }
}
```

PlatformIO build should succeed now.

## Running On Blue Pill

You will need ST Link V2 to flash your Blue Pill with the built executable.  Follow the instructions here to flash and monitor your Blue Pill:

https://medium.com/coinmonks/connect-stm32-blue-pill-to-sigfox-28c6f91bddc1?source=friends_link&sk=d0f0d9f40e36badadfb04dc4c0846775

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

## Dump of ELF Executable

```
→ objdump -t build/STM32_BLUE_PILL

build/STM32_BLUE_PILL:  file format ELF32-arm-little

SYMBOL TABLE:
00000000         *UND*           00000000
08000000 l    d  .text           00000000 .text
080030a4 l    d  .init           00000000 .init
080030a8 l    d  .fini           00000000 .fini
080030ac l    d  .preinit_array          00000000 .preinit_array
080030ac l    d  .init_array             00000000 .init_array
080030b4 l    d  .fini_array             00000000 .fini_array
080030b8 l    d  .ARM.exidx              00000000 .ARM.exidx
20000000 l    d  .data           00000000 .data
20000018 l    d  .bss            00000000 .bss
00000000 l    d  .ARM.attributes                 00000000 .ARM.attributes
00000000 l    d  .comment                00000000 .comment
00000000 l    d  .debug_info             00000000 .debug_info
00000000 l    d  .debug_abbrev           00000000 .debug_abbrev
00000000 l    d  .debug_loc              00000000 .debug_loc
00000000 l    d  .debug_aranges          00000000 .debug_aranges
00000000 l    d  .debug_ranges           00000000 .debug_ranges
00000000 l    d  .debug_line             00000000 .debug_line
00000000 l    d  .debug_str              00000000 .debug_str
00000000 l    d  .debug_frame            00000000 .debug_frame
00000000 l    df *ABS*           00000000 vector.c
08001da0         .text           00000000 $t
08001da4         .text           00000000 $t
08001da8         .text           00000000 $t
08001e64         .text           00000000 $d
08000000         .text           00000000 $d
00000000 l    df *ABS*           00000000 crtstuff.c
20000000         .data           00000000 $d
08000150         .text           00000000 $t
08000150 l     F .text           00000000 __do_global_dtors_aux
08000168         .text           00000000 $d
200000e8         .bss            00000001 completed.8655
080030b4         .fini_array             00000000 $d
080030b4 l       .fini_array             00000000 __do_global_dtors_aux_fini_array_entry
08000174         .text           00000000 $t
08000174 l     F .text           00000000 frame_dummy
08000184         .text           00000000 $d
200000ec         .bss            00000018 object.8660
080030ac         .init_array             00000000 $d
080030ac l       .init_array             00000000 __frame_dummy_init_array_entry
200000e8         .bss            00000000 $d
00000000 l    df *ABS*           00000000 main.cpp
08000190         .text           00000000 $t
08000194         .text           00000000 $d
08000198         .text           00000000 $t
0800019c         .text           00000000 $t
080001a0         .text           00000000 $t
080001b4         .text           00000000 $d
080001b8         .text           00000000 $t
080001d4         .text           00000000 $d
080001d8         .text           00000000 $t
080001d8 l     F .text           00000024 _GLOBAL__sub_I_bluepill
080001f0         .text           00000000 $d
080030b0         .init_array             00000000 $d
080001fc         .text           00000000 $t
08000224         .text           00000000 $t
08000234         .text           00000000 $d
08000238         .text           00000000 $t
08000264         .text           00000000 $d
08000270         .text           00000000 $t
080002a4         .text           00000000 $d
08002e60         .text           00000000 $d
20000018         .bss            00000000 $d
00000000 l    df *ABS*           00000000 CodalComponent.cpp
080002b0         .text           00000000 $t
080002b4         .text           00000000 $d
080002b8         .text           00000000 $t
08000324         .text           00000000 $d
08000330         .text           00000000 $t
080003f8         .text           00000000 $d
0800040c         .text           00000000 $t
08000430         .text           00000000 $d
20000104         .bss            00000000 $d
20000328         .bss            00000000 $d
00000000 l    df *ABS*           00000000 CodalFiber.cpp
08000434         .text           00000000 $t
08000438         .text           00000000 $d
0800043c         .text           00000000 $t
0800043c l     F .text           0000002e _ZN5codal13dequeue_fiberEPNS_5FiberE.part.0
0800046c         .text           00000000 $t
080004a4         .text           00000000 $t
080004f8         .text           00000000 $d
08000500         .text           00000000 $t
080005d4         .text           00000000 $d
080005e8         .text           00000000 $t
08000638         .text           00000000 $d
0800063c         .text           00000000 $t
08000758         .text           00000000 $d
0800077c         .text           00000000 $t
08000788         .text           00000000 $d
0800078c         .text           00000000 $t
080007e8         .text           00000000 $d
080007f4         .text           00000000 $t
08000800         .text           00000000 $d
08000804         .text           00000000 $t
08000844         .text           00000000 $d
0800084c         .text           00000000 $t
0800084c l     F .text           00000170 _ZN5codal8scheduleEv.part.3
080009a4         .text           00000000 $d
080009bc         .text           00000000 $t
080009cc         .text           00000000 $d
080009d0         .text           00000000 $t
08000a38         .text           00000000 $d
08000a48         .text           00000000 $t
08000a48 l     F .text           00000048 _ZN5codal13release_fiberEv.part.4
08000a84         .text           00000000 $d
08000a90         .text           00000000 $t
08000aac         .text           00000000 $d
08000ab0         .text           00000000 $t
08000ac0         .text           00000000 $d
08000ac4         .text           00000000 $t
08000b4c         .text           00000000 $d
08000b5c         .text           00000000 $t
08000b80         .text           00000000 $d
08000b84         .text           00000000 $t
08000b98         .text           00000000 $d
200001f4         .bss            00000000 $d
200001f8         .bss            00000000 $d
200001f8 l       .bss            00000004 _ZN5codalL10messageBusE
200001fc         .bss            00000000 $d
200001fc l       .bss            00000004 _ZN5codalL10sleepQueueE
20000329 l       .bss            00000001 _ZN5codalL11fiber_flagsE
20000329         .bss            00000000 $d
20000200         .bss            00000000 $d
20000200 l       .bss            00000004 _ZN5codalL11forkedFiberE
20000204         .bss            00000000 $d
20000204 l       .bss            00000004 _ZN5codalL8runQueueE
20000208         .bss            00000000 $d
20000208 l       .bss            00000004 _ZN5codalL9fiberPoolE
2000020c         .bss            00000000 $d
2000020c l       .bss            00000004 _ZN5codalL9idleFiberE
20000210         .bss            00000000 $d
20000210 l       .bss            00000004 _ZN5codalL9waitQueueE
00000000 l    df *ABS*           00000000 CodalHeapAllocator.cpp
08000b9c         .text           00000000 $t
08000bd0         .text           00000000 $d
08000bd8         .text           00000000 $t
08000c68         .text           00000000 $t
08000ca0         .text           00000000 $d
08000cb4         .text           00000000 $t
08000cf0         .text           00000000 $d
08000cf4         .text           00000000 $t
2000032a l       .bss            00000001 _ZZ6mallocE11initialised
2000032a         .bss            00000000 $d
20000214         .bss            00000000 $d
2000032b         .bss            00000000 $d
00000000 l    df *ABS*           00000000 CodalListener.cpp
08000d10         .text           00000000 $t
08000d40         .text           00000000 $t
08000d54         .text           00000000 $t
00000000 l    df *ABS*           00000000 Timer.cpp
08000dec         .text           00000000 $t
08000df8         .text           00000000 $t
08000e04         .text           00000000 $t
08000e34         .text           00000000 $d
08000e3c         .text           00000000 $t
08000f0c         .text           00000000 $d
08000f14         .text           00000000 $t
08000f18         .text           00000000 $t
08000f34         .text           00000000 $d
08000f38         .text           00000000 $t
08000f54         .text           00000000 $d
08000f58         .text           00000000 $t
08000f80         .text           00000000 $d
2000021c         .bss            00000000 $d
08002e7c         .text           00000000 $d
00000000 l    df *ABS*           00000000 MessageBus.cpp
08000f88         .text           00000000 $t
08000f94         .text           00000000 $d
08000f98         .text           00000000 $t
08000fb0         .text           00000000 $t
08000ffc         .text           00000000 $t
0800107c         .text           00000000 $d
08001080         .text           00000000 $t
08001164         .text           00000000 $d
08001168         .text           00000000 $t
08001208         .text           00000000 $t
0800127c         .text           00000000 $d
0800128c         .text           00000000 $t
08001294         .text           00000000 $t
080012a8         .text           00000000 $t
080012b0         .text           00000000 $t
08001344         .text           00000000 $d
08001360         .text           00000000 $t
08001384         .text           00000000 $t
080013d8         .text           00000000 $t
08001474         .text           00000000 $d
08001478         .text           00000000 $t
080014e8         .text           00000000 $t
08001500         .text           00000000 $t
08002e94         .text           00000000 $d
08002ea8         .text           00000000 $d
00000000 l    df *ABS*           00000000 Event.cpp
08001548         .text           00000000 $t
08001548 l     F .text           00000038 _ZN5codal5EventC2EttNS_15EventLaunchModeE.part.0
08001578         .text           00000000 $d
08001548 l     F .text           00000038 _ZN5codal5EventC2EttyNS_15EventLaunchModeE.part.1
08001580         .text           00000000 $t
080015a0         .text           00000000 $t
080015b4         .text           00000000 $t
20000220         .bss            00000000 $d
00000000 l    df *ABS*           00000000 MemberFunctionCallback.cpp
080015dc         .text           00000000 $t
0800160c         .text           00000000 $t
00000000 l    df *ABS*           00000000 STM32BluePill.cpp
0800162c         .text           00000000 $t
08001630         .text           00000000 $t
08001634         .text           00000000 $t
08001684         .text           00000000 $d
08001690         .text           00000000 $t
080016d4         .text           00000000 $d
08002ee4         .text           00000000 $d
00000000 l    df *ABS*           00000000 STM32BluePillIO.cpp
080016dc         .text           00000000 $t
00000000 l    df *ABS*           00000000 bluepill.cpp
08001718         .text           00000000 $t
0800175c         .text           00000000 $d
08001764         .text           00000000 $t
08001770         .text           00000000 $d
08001774         .text           00000000 $t
0800177c         .text           00000000 $t
08001780         .text           00000000 $t
2000032c l       .bss            00000001 _ZL12debugEnabled
2000032c         .bss            00000000 $d
08002f00         .text           00000000 $d
00000000 l    df *ABS*           00000000 led.cpp
08001784         .text           00000000 $t
080017a0         .text           00000000 $d
080017a4         .text           00000000 $t
080017b0         .text           00000000 $d
080017b4         .text           00000000 $t
080017c0         .text           00000000 $d
080017c4         .text           00000000 $t
080017d0         .text           00000000 $d
00000000 l    df *ABS*           00000000 CmI2C.cpp
080017d4         .text           00000000 $t
080017d8         .text           00000000 $t
080017dc         .text           00000000 $t
080017e0         .text           00000000 $t
080017f8         .text           00000000 $d
08002f14         .text           00000000 $d
00000000 l    df *ABS*           00000000 CmPin.cpp
080017fc         .text           00000000 $t
08001804         .text           00000000 $t
0800180c         .text           00000000 $t
08001814         .text           00000000 $t
08001818         .text           00000000 $t
08001840         .text           00000000 $d
08001844         .text           00000000 $t
08001848         .text           00000000 $t
08001858         .text           00000000 $t
08001868         .text           00000000 $t
08001874         .text           00000000 $t
08001884         .text           00000000 $t
08001898         .text           00000000 $d
0800189c         .text           00000000 $t
080018a4         .text           00000000 $t
080018f4         .text           00000000 $d
080018f8         .text           00000000 $t
08001940         .text           00000000 $d
08001948         .text           00000000 $t
08001958         .text           00000000 $d
0800195c         .text           00000000 $t
08001966         .text           00000000 $d
0800196a         .text           00000000 $t
08001990         .text           00000000 $t
080019c0         .text           00000000 $d
080019c8         .text           00000000 $t
080019e0         .text           00000000 $d
080019e4         .text           00000000 $t
08001a20         .text           00000000 $t
08001a60         .text           00000000 $d
08001a68         .text           00000000 $t
08001a8c         .text           00000000 $t
08002f3c         .text           00000000 $d
00000000 l    df *ABS*           00000000 CmTimer.cpp
08001a98         .text           00000000 $t
08001a9c         .text           00000000 $t
08001aa0         .text           00000000 $t
08001ab0         .text           00000000 $d
08002f90         .text           00000000 $d
00000000 l    df *ABS*           00000000 DynamicPwm.cpp
08001ab4         .text           00000000 $t
08001abc         .text           00000000 $t
08001ac0         .text           00000000 $t
08001ac4         .text           00000000 $t
08001ac8         .text           00000000 $t
00000000 l    df *ABS*           00000000 codal_target_hal.cpp
08001acc         .text           00000000 $t
08001adc         .text           00000000 $d
08001ae0         .text           00000000 $t
08001af0         .text           00000000 $d
08001af4         .text           00000000 $t
08001af8         .text           00000000 $t
08001afc         .text           00000000 $t
08001b1c         .text           00000000 $d
08001b24         .text           00000000 $t
08001b28         .text           00000000 $d
08001b2c         .text           00000000 $t
08001b34         .text           00000000 $t
08001b38         .text           00000000 $t
08001b3c         .text           00000000 $t
08001b40         .text           00000000 $t
08001b44         .text           00000000 $t
08001b48         .text           00000000 $t
08001b4c         .text           00000000 $t
08001b54         .text           00000000 $t
08001b98         .text           00000000 $d
20000004         .data           00000000 $d
08002fa8         .text           00000000 $d
08002fc0         .text           00000000 $d
08002fd8         .text           00000000 $d
08002fec         .text           00000000 $d
00000000 l    df *ABS*           00000000 logger.cpp
08001bb0         .text           00000000 $t
08001bb0 l     F .text           00000040 debug_flush.part.1
08001be8         .text           00000000 $d
08001bf0         .text           00000000 $t
08001bf0 l     F .text           00000064 _ZL12debug_appendPKcj
08001c4c         .text           00000000 $d
08001c54         .text           00000000 $t
08001c54 l     F .text           00000014 debug_print.part.3
08001c68         .text           00000000 $t
08001c78         .text           00000000 $d
08001c80         .text           00000000 $t
08001c8c         .text           00000000 $d
08001c90         .text           00000000 $t
08001d0c         .text           00000000 $d
08001d18         .text           00000000 $t
08001d40         .text           00000000 $d
08001d48         .text           00000000 $t
08001d54         .text           00000000 $t
08001d6c         .text           00000000 $d
08001d70         .text           00000000 $t
08001d84         .text           00000000 $d
08001d88         .text           00000000 $t
08001d9c         .text           00000000 $d
2000032d l       .bss            00000001 _ZL10logEnabled
2000032d         .bss            00000000 $d
20000224         .bss            00000000 $d
20000224 l       .bss            00000051 _ZL11debugBuffer
08003020         .text           00000000 $d
08003028         .text           00000000 $d
08003034         .text           00000000 $d
00000000 l    df *ABS*           00000000 gpio_common_all.c
08001e90         .text           00000000 $t
08001e94         .text           00000000 $t
00000000 l    df *ABS*           00000000 rcc_common_all.c
08001e9c         .text           00000000 $t
00000000 l    df *ABS*           00000000 gpio.c
08001eb8         .text           00000000 $t
00000000 l    df *ABS*           00000000 rcc.c
08001f34         .text           00000000 $t
08001f34 l     F .text           00000010 rcc_is_osc_ready.part.0
08001f40         .text           00000000 $d
08001f44         .text           00000000 $t
08001f44 l     F .text           00000010 rcc_osc_on.part.1
08001f50         .text           00000000 $d
08001f54         .text           00000000 $t
08002004         .text           00000000 $d
20000008         .data           00000000 $d
2000000c         .data           00000000 $d
20000010         .data           00000000 $d
00000000 l    df *ABS*           00000000 flash_common_all.c
08002020         .text           00000000 $t
08002030         .text           00000000 $d
00000000 l    df *ABS*           00000000 _arm_addsubsf3.o
08002034         .text           00000000 $t
00000000 l    df *ABS*           00000000 _arm_muldivsf3.o
08002250         .text           00000000 $t
00000000 l    df *ABS*           00000000 _aeabi_uldivmod.o
080024f0         .text           00000000 $t
00000000 l    df *ABS*           00000000 libgcc2.c
08002520         .text           00000000 $t
080030b8         .ARM.exidx              00000000 $d
00000000 l    df *ABS*           00000000 _dvmd_tls.o
080027f0         .text           00000000 $t
00000000 l    df *ABS*           00000000 cxa_atexit.c
080027f4         .text           00000000 $t
0800280c         .text           00000000 $d
00000000 l    df *ABS*           00000000 CortexContextSwitch.s.o
08002810         .text           00000000 $t
08002840         .text           00000000 store_context_complete
08002852         .text           00000000 store_stack_complete
08002846         .text           00000000 store_stack
0800286c         .text           00000000 restore_stack_complete
08002860         .text           00000000 restore_stack
080028c0         .text           00000000 store_stack1
00000000 l    df *ABS*           00000000 CodalDmesg.cpp
08002934         .text           00000000 $t
0800293c         .text           00000000 $d
08002940         .text           00000000 $t
0800294c         .text           00000000 $d
20000278         .bss            00000000 $d
20000278 l       .bss            00000004 _ZL14dmesg_flush_fn
00000000 l    df *ABS*           00000000 I2C.cpp
08002950         .text           00000000 $t
08002958         .text           00000000 $t
08002960         .text           00000000 $t
08002968         .text           00000000 $t
080029b8         .text           00000000 $t
08002a44         .text           00000000 $d
08002a4c         .text           00000000 $t
08002b18         .text           00000000 $d
08002b20         .text           00000000 $t
08002b28         .text           00000000 $d
08003038         .text           00000000 $d
00000000 l    df *ABS*           00000000 Button.cpp
08002b2c         .text           00000000 $t
08002b34         .text           00000000 $t
08002b3c         .text           00000000 $t
08002b44         .text           00000000 $t
08002b54         .text           00000000 $d
08002b58         .text           00000000 $t
08002b70         .text           00000000 $d
08002b74         .text           00000000 $t
08002c5c         .text           00000000 $t
08002c80         .text           00000000 $d
08002c84         .text           00000000 $t
08002ccc         .text           00000000 $d
08003060         .text           00000000 $d
00000000 l    df *ABS*           00000000 AbstractButton.cpp
08002cd4         .text           00000000 $t
08002cd8         .text           00000000 $t
08002cec         .text           00000000 $d
08002cf0         .text           00000000 $t
08002d0c         .text           00000000 $d
08002d10         .text           00000000 $t
08002d34         .text           00000000 $d
08003084         .text           00000000 $d
00000000 l    df *ABS*           00000000 os_kernel.c
08002d3c         .text           00000000 $t
08002d64         .text           00000000 $d
00000000 l    df *ABS*           00000000 os_msgqueue.c
08002d70         .text           00000000 $t
08002d8c         .text           00000000 $d
2000027c         .bss            00000000 $d
2000027c l       .bss            00000020 msgQList
2000032e l       .bss            00000001 nQueues
2000032e         .bss            00000000 $d
00000000 l    df *ABS*           00000000 os_sem.c
08002d94         .text           00000000 $t
08002d9c         .text           00000000 $d
2000032f l       .bss            00000001 nSemaphores
2000032f         .bss            00000000 $d
00000000 l    df *ABS*           00000000 os_task.c
08002da0         .text           00000000 $t
08002ddc         .text           00000000 $d
20000330 l       .bss            00000001 nTasks
20000330         .bss            00000000 $d
2000029c         .bss            00000000 $d
2000029c l       .bss            0000008c task_list
00000000 l    df *ABS*           00000000 os_event.c
08002de4         .text           00000000 $t
08002dec         .text           00000000 $d
20000331 l       .bss            00000001 nEvents
20000331         .bss            00000000 $d
00000000 l    df *ABS*           00000000 atexit_arm.cc
08002df0         .text           00000000 $t
00000000 l    df *ABS*           00000000 memcmp.c
08002dfa         .text           00000000 $t
00000000 l    df *ABS*           00000000 memset.c
08002e18         .text           00000000 $t
00000000 l    df *ABS*           00000000 lib_a-strlen.o
08002e28         .text           00000000 $t
00000000 l    df *ABS*           00000000 strncat.c
08002e38         .text           00000000 $t
00000000 l    df *ABS*           00000000 /usr/local/Cellar/arm-none-eabi-gcc/7-2018-q2-update/gcc/bin/../lib/gcc/arm-none-eabi/7.3.1/thumb/v7-m/crti.o
080030a4         .init           00000000 $t
080030a8         .fini           00000000 $t
08000e04 g     F .text           00000038 _ZN5codal5TimerC1Ev
08001294 g     F .text           00000012 _ZN5codal10MessageBusD0Ev
08000f58 g     F .text           00000030 _ZN5codal27system_timer_event_every_usEytt
08000198 gw    F .text           00000002 _ZN5codal14CodalComponent16periodicCallbackEv
08000238 gw    F .text           00000038 _ZN5codal13STM32BluePillD1Ev
08002cd8 g     F .text           00000018 _ZN5codal14AbstractButtonD2Ev
080004a4 g     F .text           0000005c _ZN5codal14scheduler_tickENS_5EventE
08002c84 g     F .text           00000050 _ZN5codal6ButtonC1ERNS_3PinEt24ButtonEventConfiguration14ButtonPolarityNS_8PullModeE
08001b3c g     F .text           00000004 tcb_configure_stack_base
08001da8 gw    F .text           000000e8 reset_handler
08001b24 g     F .text           00000008 fiber_initial_stack_base
2000032b g       .bss            00000001 heap_count
08001858 g     F .text           0000000e _ZN5codal3_cm3Pin8isOutputEv
08000d40 g     F .text           00000014 _ZN5codal8ListenerD2Ev
08001b38 g     F .text           00000004 tcb_configure_sp
08001da0 gw    F .text           00000002 usart3_isr
08000238 gw    F .text           00000038 _ZN5codal13STM32BluePillD2Ev
08001da0 gw    F .text           00000002 rtc_isr
08002f90 g       .text           00000018 _ZTVN5codal3_cm5TimerE
08001d18 g     F .text           00000030 _Z11debug_printi
080012b0 g     F .text           000000b0 _ZN5codal10MessageBusC2Ev
08001d70 g     F .text           00000018 _Z13debug_printlni
08001da0 gw    F .text           00000002 tim7_isr
08002cd4 g     F .text           00000004 _ZN5codal14AbstractButton9isPressedEv
08001ac4 g     F .text           00000004 _ZN10DynamicPwm11getPeriodUsEv
20000332 g       .bss            00000001 running_tid
08001da0 gw    F .text           00000002 adc1_2_isr
080015b4 g     F .text           00000028 _ZN5codal14EventQueueItemC2ENS_5EventE
080023b8 g     F .text           00000136 .hidden __divsf3
08002ee4 g       .text           0000001c _ZTVN5codal13STM32BluePillE
08001b44 g     F .text           00000004 get_current_sp
08001da0 gw    F .text           00000002 tim1_trg_com_isr
080017dc g     F .text           00000004 _ZN5codal3_cm3I2C12readRegisterEthPhib
08002968 g     F .text           0000004e _ZN5codal3I2C12readRegisterEthPhib
0800040c g     F .text           00000028 _ZN5codal14CodalComponent15removeComponentEv
080030c0         .ARM.exidx              00000000 __exidx_end
080019e4 g     F .text           0000003a _ZN5codal3_cm3Pin19obtainAnalogChannelEv
08001da0 gw    F .text           00000002 usb_hp_can_tx_isr
08000e3c g     F .text           000000d8 _ZN5codal5Timer8setEventEyttb
080001a0 gw    F .text           00000018 _ZN5codal14CodalComponentD2Ev
08002a4c g     F .text           000000d4 _ZN5codal3I2C4readEtPhib
08002b2c gw    F .text           00000006 _ZN5codal3Pin15getDigitalValueEv
08001da0 gw    F .text           00000002 tim6_isr
080030c0         .data           00000000 _etext
08001eb8 g     F .text           0000007c gpio_set_mode
0800046c g     F .text           00000036 _ZN5codal11queue_fiberEPNS_5FiberEPS1_
08001da0 gw    F .text           00000002 usb_wakeup_isr
08001da0 g     F .text           00000002 blocking_handler
20000333 g       .bss            00000001 running
08001784 g     F .text           00000020 led_setup
08002040 g     F .text           00000160 .hidden __aeabi_fadd
08001da0 gw    F .text           00000002 tim5_isr
0800203c g     F .text           00000164 .hidden __subsf3
08001d54 g     F .text           0000001c debug_println
08001da0 gw    F .text           00000002 otg_fs_isr
08001b54 g     F .text           0000005c test_codal
08001844 g     F .text           00000004 _ZN5codal3_cm3Pin14getAnalogValueEv
08001da0 gw    F .text           00000002 spi1_isr
08000b9c g     F .text           0000003c _Z18device_create_heapmm
08002b44 g     F .text           00000014 _ZN5codal6ButtonD2Ev
0800195c g     F .text           00000034 _ZN5codal3_cm3Pin7eventOnEi
08001ac8 g     F .text           00000004 _ZN10DynamicPwm11setPeriodUsEm
08001da0 gw    F .text           00000002 exti2_isr
08001da0 gw    F .text           00000002 dma1_channel6_isr
08001da4 g     F .text           00000002 null_handler
20000008 g       .data           00000004 rcc_ahb_frequency
08001690 g     F .text           0000004c _ZN5codal13STM32BluePillC2Ev
080015dc g     F .text           00000030 _ZN5codal22MemberFunctionCallback4fireENS_5EventE
080024f0 g     F .text           00000000 .hidden __aeabi_uldivmod
08001b4c g     F .text           00000008 tcb_configure_args
08001da0 gw    F .text           00000002 can_rx1_isr
080030b8         .fini_array             00000000 __fini_array_end
08001da0 gw    F .text           00000002 dma1_channel5_isr
08000c68 g     F .text           0000004c malloc
08001da0 gw    F .text           00000002 dma2_channel5_isr
08002934 g     F .text           0000000c codal_dmesg_set_flush_fn
08000d40 g     F .text           00000014 _ZN5codal8ListenerD1Ev
080021a8 g     F .text           0000001c .hidden __aeabi_i2f
08000bd8 g     F .text           0000008e _Z13device_mallocjR14HeapDefinition
0800078c g     F .text           00000068 _Z14__create_fibermmmi
08002940 g     F .text           00000010 codal_dmesg_flush
08001da0 gw    F .text           00000002 usart1_isr
08000804 g     F .text           00000048 _ZN5codal4idleEv
20000000 g       .data           00000000 .hidden __dso_handle
08001b48 g     F .text           00000004 tcb_get_sp
08001948 g     F .text           00000014 _ZN5codal3_cm3Pin17getAnalogPeriodUsEv
08002520 g     F .text           000002ce .hidden __udivmoddi4
080017d8 g     F .text           00000004 _ZN5codal3_cm3I2C4readEtPhib
08000ab0 g     F .text           00000014 _ZN5codal13release_fiberEPv
0800180c gw    F .text           00000006 _ZN5codal3Pin8drainPinEv
08001da0 gw    F .text           00000002 usage_fault_handler
08001da0 gw    F .text           00000002 tim8_trg_com_isr
08001aa0 g     F .text           00000014 _ZN5codal3_cm5TimerC1Ev
08001da0 gw    F .text           00000002 can2_rx0_isr
08002960 g     F .text           00000006 _ZN5codal3I2C4readENS_15AcknowledgeTypeE
08001da0 gw    F .text           00000002 tim1_brk_isr
080013d8 g     F .text           000000a0 _ZN5codal10MessageBus7processERNS_5EventEb
080001a0 gw    F .text           00000018 _ZN5codal14CodalComponentD1Ev
08002d10 g     F .text           0000002c _ZN5codal14AbstractButtonC2Ev
08001c80 g     F .text           00000010 debug_flush
08002950 g     F .text           00000006 _ZN5codal3I2C12setFrequencyEm
080009d0 g     F .text           00000078 _ZN5codal11fiber_sleepEm
080014e8 g     F .text           00000018 _ZN5codal10MessageBus4sendENS_5EventE
08002b58 g     F .text           0000001c _ZN5codal6ButtonD0Ev
080030b8         .ARM.exidx              00000000 __exidx_start
08001da0 gw    F .text           00000002 can2_rx1_isr
08002f14 g       .text           00000028 _ZTVN5codal3_cm3I2CE
08002c84 g     F .text           00000050 _ZN5codal6ButtonC2ERNS_3PinEt24ButtonEventConfiguration14ButtonPolarityNS_8PullModeE
08001780 g     F .text           00000004 _ZdlPv
080019c8 g     F .text           0000001c _ZN5codal3_cm3PinC1EihNS_13PinCapabilityE
08001da0 gw    F .text           00000002 tim1_cc_isr
08001884 g     F .text           00000018 _ZN5codal3_cm3Pin15getAnalogPeriodEv
08001e9c g     F .text           0000001c rcc_periph_clock_enable
08001da0 gw    F .text           00000002 sdio_isr
08002cd8 g     F .text           00000018 _ZN5codal14AbstractButtonD1Ev
080030a4 g     F .init           00000000 _init
08001da0 gw    F .text           00000002 eth_isr
08002960 g     F .text           00000006 _ZN5codal3I2C5writeEh
2000000c g       .data           00000004 rcc_apb1_frequency
20000328 g       .bss            00000001 _ZN5codal14CodalComponent13configurationE
08001da0 gw    F .text           00000002 dma1_channel4_isr
08001da0 gw    F .text           00000002 tim8_brk_isr
08000f18 g     F .text           00000020 _ZN5codal25system_timer_current_timeEv
08001da0 gw    F .text           00000002 dma2_channel4_5_isr
080015b4 g     F .text           00000028 _ZN5codal14EventQueueItemC1ENS_5EventE
200001f4 g       .bss            00000004 _ZN5codal12currentFiberE
08000f14 g     F .text           00000002 _ZN5codal5TimerD2Ev
0800203c g     F .text           00000164 .hidden __aeabi_fsub
20000220 g       .bss            00000004 _ZN5codal10EventModel15defaultEventBusE
080007f4 g     F .text           00000010 _ZN5codal24scheduler_runqueue_emptyEv
080002b8 g     F .text           00000078 _Z18component_callbackN5codal5EventE
20000338         .bss            00000000 _ebss
08001abc g     F .text           00000004 _ZN10DynamicPwm5writeEf
08001580 g     F .text           0000001e _ZN5codal5EventC1EttNS_15EventLaunchModeE
08002d3c g     F .text           00000034 os_init
08002906         .text           00000000 restore_register_context
08000190 gw    F .text           00000008 _ZN5codal14CodalComponent4initEv
08001da0 gw    F .text           00000002 pvd_isr
08002b20 g     F .text           0000000c _ZN5codal3I2CC2ERNS_3PinES2_
08000cf4 g     F .text           0000001c calloc
08001718 g     F .text           0000004c platform_setup
080021d4 g     F .text           0000007c .hidden __floatdisf
08000434 gw    F .text           00000008 _ZN5codal10EventModel6removeEPNS_8ListenerE
08002de4 g     F .text           0000000c os_event_init
080016dc g     F .text           0000003c _ZN5codal15STM32BluePillIOC2Ev
08001ae0 g     F .text           00000014 target_disable_irq
080017a4 g     F .text           00000010 led_on
08000a90 g     F .text           00000020 _ZN5codal16launch_new_fiberEPFvvES1_
20000334 g       .bss            00000001 last_running_task
08001208 g     F .text           00000084 _ZN5codal10MessageBusD2Ev
08001da4 gw    F .text           00000002 sv_call_handler
20000018 g       .bss            000000d0 bluepill
08001da0 gw    F .text           00000002 rcc_isr
08000000 g       .text           00000150 vector_table
080028d6         .text           00000000 save_register_context
08001da0 gw    F .text           00000002 flash_isr
080030c0         *ABS*           00000000 _data_loadaddr
20000338         .bss            00000000 end
08001da0 gw    F .text           00000002 uart4_isr
08001634 g     F .text           0000005c _ZN5codal13STM32BluePill4initEv
080018f8 g     F .text           00000050 _ZN5codal3_cm3Pin15setDigitalValueEi
08001da0 gw    F .text           00000002 rtc_alarm_isr
08001da0 gw    F .text           00000002 exti15_10_isr
0800077c g     F .text           00000010 _ZN5codal23fiber_scheduler_runningEv
08001814 g     F .text           00000004 _ZN5codal3_cm3Pin15getDigitalValueEv
08001b40 g     F .text           00000004 tcb_get_stack_base
080015a0 g     F .text           00000014 _ZN5codal5EventC2Ev
08001630 g     F .text           00000004 _ZN5codal13STM32BluePill12idleCallbackEv
08001b2c g     F .text           00000006 tcb_allocate
08002e7c g       .text           00000018 _ZTVN5codal5TimerE
08001da0 gw    F .text           00000002 hard_fault_handler
080029b8 g     F .text           00000094 _ZN5codal3I2C5writeEtPhib
08001da0 gw    F .text           00000002 exti1_isr
0800189c g     F .text           00000006 _ZN5codal3_cm3Pin7setPullENS_8PullModeE
08002cf0 g     F .text           00000020 _ZN5codal14AbstractButtonD0Ev
08001a98 g     F .text           00000002 _ZN5codal3_cm5Timer4initEv
08001da0 gw    F .text           00000002 i2c1_ev_isr
08001ab4 g     F .text           00000006 _ZN10DynamicPwmC1E7PinName
08002c5c g     F .text           00000028 _ZN5codal6Button12buttonActiveEv
08001da0 gw    F .text           00000002 dma2_channel1_isr
08000fb0 gw    F .text           0000004c _ZN5codal22MemberFunctionCallback10methodCallINS_10MessageBusEEEvPvPmNS_5EventE
080002b0 gw    F .text           00000008 _ZN5codal10EventModel3addEPNS_8ListenerE
08001c68 g     F .text           00000018 enable_log
08001da4 gw    F .text           00000002 pend_sv_handler
08001774 g     F .text           00000008 __cxa_pure_virtual
08000500 g     F .text           000000e8 _ZN5codal15scheduler_eventENS_5EventE
08001da0 gw    F .text           00000002 spi2_isr
080021a8 g     F .text           0000001c .hidden __floatsisf
08001a20 g     F .text           00000048 _ZN5codal3_cm3Pin14setAnalogValueEi
0800063c g     F .text           00000140 _ZN5codal14scheduler_initERNS_10EventModelE
08002b34 gw    F .text           00000006 _ZN5codal3Pin7setPullENS_8PullModeE
080017d8 g     F .text           00000004 _ZN5codal3_cm3I2C5writeEtPhib
08001da0 gw    F .text           00000002 tim8_up_isr
08002d94 g     F .text           0000000c os_sem_init
08000270 gw    F .text           00000040 _ZN5codal13STM32BluePillD0Ev
080027f4 g     F .text           0000001c __cxa_atexit
080023b8 g     F .text           00000136 .hidden __aeabi_fdiv
08001da0 gw    F .text           00000002 dma2_channel2_isr
08002020 g     F .text           00000014 flash_set_ws
08002e60 gw      .text           0000001c _ZTVN5codal14CodalComponentE
08002dfa g     F .text           0000001e memcmp
080021c4 g     F .text           0000008c .hidden __floatundisf
08001874 g     F .text           0000000e _ZN5codal3_cm3Pin8isAnalogEv
080027f0 gw    F .text           00000002 .hidden __aeabi_ldiv0
08001da4 gw    F .text           00000002 debug_monitor_handler
08001da0 gw    F .text           00000002 exti3_isr
08001da0 gw    F .text           00000002 adc3_isr
08002e38 g     F .text           00000028 strncat
080001b8 gw    F .text           00000020 _ZN5codal14CodalComponentD0Ev
08002892         .text           00000000 save_context
08001da0 gw    F .text           00000002 tim3_isr
08001da0 gw    F .text           00000002 usart2_isr
080021a0 g     F .text           00000024 .hidden __aeabi_ui2f
08001afc gw    F .text           00000028 target_panic
08001d48 g     F .text           0000000a debug_print
08001da0 gw    F .text           00000002 usb_lp_can_rx0_isr
08002810         .text           00000000 swap_context
080030b4         .fini_array             00000000 __fini_array_start
08001da0 gw    F .text           00000002 i2c2_er_isr
08002e18 g     F .text           00000010 memset
2000021c g       .bss            00000004 _ZN5codal12system_timerE
08000224 g     F .text           00000014 main
20000104 g       .bss            000000f0 _ZN5codal14CodalComponent10componentsE
08002b20 g     F .text           0000000c _ZN5codal3I2CC1ERNS_3PinES2_
08001da0 gw    F .text           00000002 i2c2_ev_isr
080001fc g     F .text           00000026 _Z10Blink_mainRN5codal13STM32BluePillE
080021d4 g     F .text           0000007c .hidden __aeabi_l2f
08001da0 gw    F .text           00000002 uart5_isr
20000214 g       .bss            00000008 heap
08001da4 gw    F .text           00000002 sys_tick_handler
08002e94 gw      .text           00000014 _ZTVN5codal10EventModelE
08001208 g     F .text           00000084 _ZN5codal10MessageBusD1Ev
080030b4         .init_array             00000000 __init_array_end
08001da0 gw    F .text           00000002 fsmc_isr
08001da0 gw    F .text           00000002 dma1_channel1_isr
08001da0 gw    F .text           00000002 exti4_isr
08001aa0 g     F .text           00000014 _ZN5codal3_cm5TimerC2Ev
08002f3c g       .text           00000054 _ZTVN5codal3_cm3PinE
20000004 g       .data           00000004 codal_heap_start
08000e04 g     F .text           00000038 _ZN5codal5TimerC2Ev
08001af4 g     F .text           00000002 target_wait_for_event
08000d10 g     F .text           0000002e _ZN5codal8ListenerC2EttPFvNS_5EventEEt
08001a68 g     F .text           00000024 _ZN5codal3_cm3Pin17setAnalogPeriodUsEi
08001c90 g     F .text           00000088 _Z11debug_printj
080021c4 g     F .text           0000008c .hidden __aeabi_ul2f
08001d88 g     F .text           00000018 _Z13debug_printlnj
080017d4 g     F .text           00000004 _ZN5codal3_cm3I2C12setFrequencyEm
08002250 g     F .text           00000168 .hidden __mulsf3
080012b0 g     F .text           000000b0 _ZN5codal10MessageBusC1Ev
08000ffc g     F .text           00000084 _ZN5codal10MessageBus6removeEPNS_8ListenerE
08001da0 gw    F .text           00000002 mem_manage_handler
08001848 g     F .text           0000000e _ZN5codal3_cm3Pin7isInputEv
08001868 g     F .text           0000000c _ZN5codal3_cm3Pin9isDigitalEv
20000010 g       .data           00000004 rcc_apb2_frequency
080015a0 g     F .text           00000014 _ZN5codal5EventC1Ev
08002d10 g     F .text           0000002c _ZN5codal14AbstractButtonC1Ev
08002958 g     F .text           00000006 _ZN5codal3I2C4stopEv
080030a8 g     F .fini           00000000 _fini
08000f38 g     F .text           00000020 _ZN5codal28system_timer_current_time_usEv
08001af8 g     F .text           00000002 target_wait
08001da0 gw    F .text           00000002 can2_tx_isr
080017e0 g     F .text           0000001c _ZN5codal3_cm3I2CC1ERNS_3PinES3_
08000ac4 g     F .text           00000098 _ZN5codal6invokeEPFvPvES0_
08001da0 gw    F .text           00000002 exti9_5_isr
080017b4 g     F .text           00000010 led_off
08002b44 g     F .text           00000014 _ZN5codal6ButtonD1Ev
080017fc gw    F .text           00000006 _ZN5codal3Pin15setServoPulseUsEm
08001da0 gw    F .text           00000002 dma2_channel3_isr
08002034 g     F .text           0000016c .hidden __aeabi_frsub
08001168 g     F .text           000000a0 _Z14async_callbackPv
08003084 g       .text           00000020 _ZTVN5codal14AbstractButtonE
08002250 g     F .text           00000168 .hidden __aeabi_fmul
0800019c gw    F .text           00000002 _ZN5codal14CodalComponent12idleCallbackEv
080030ac         .preinit_array          00000000 __preinit_array_end
080017c4 g     F .text           00000010 led_wait
080016dc g     F .text           0000003c _ZN5codal15STM32BluePillIOC1Ev
08001da0 gw    F .text           00000002 dma1_channel7_isr
08000d10 g     F .text           0000002e _ZN5codal8ListenerC1EttPFvNS_5EventEEt
20000000         .data           00000000 _data
08001da0 gw    F .text           00000002 tim1_up_isr
08003060 g       .text           00000024 _ZTVN5codal6ButtonE
080019c8 g     F .text           0000001c _ZN5codal3_cm3PinC2EihNS_13PinCapabilityE
08001a98 g     F .text           00000002 _ZN5codal3_cm5Timer11syncRequestEv
08001990 g     F .text           00000038 _ZN5codal3_cm3Pin15getDigitalValueENS_8PullModeE
080017e0 g     F .text           0000001c _ZN5codal3_cm3I2CC2ERNS_3PinES3_
08001f54 g     F .text           000000cc rcc_clock_setup_in_hse_8mhz_out_72mhz
08002b3c g     F .text           00000008 _ZN5codal6Button9isPressedEv
0800160c g     F .text           00000020 _ZN5codal22MemberFunctionCallbackeqERKS0_
08001804 gw    F .text           00000006 _ZN5codal3Pin17setAnalogPeriodUsEm
08001a8c g     F .text           0000000c _ZN5codal3_cm3Pin15setAnalogPeriodEi
08001da0 gw    F .text           00000002 can2_sce_isr
08001da0 gw    F .text           00000002 tim4_isr
08003038 g       .text           00000028 _ZTVN5codal3I2CE
08001360 g     F .text           00000024 _ZN5codal10MessageBus12dequeueEventEv
08001384 g     F .text           00000054 _ZN5codal10MessageBus21deleteMarkedListenersEv
20000014         .data           00000000 _edata
08000b5c g     F .text           00000028 _ZN5codal22launch_new_fiber_paramEPFvPvES2_S0_
08001080 g     F .text           000000e8 _ZN5codal10MessageBus3addEPNS_8ListenerE
08001da0 gw    F .text           00000002 dma1_channel2_isr
08001da0 gw    F .text           00000002 i2c1_er_isr
08002b74 g     F .text           000000e6 _ZN5codal6Button16periodicCallbackEv
080005e8 g     F .text           00000054 _Z15getFiberContextv
08001acc g     F .text           00000014 target_enable_irq
08000dec g     F .text           0000000a _ZN5codal5Timer17disableInterruptsEv
08001da0 gw    F .text           00000002 can_sce_isr
08001da4 gw    F .text           00000002 nmi_handler
0800177c g     F .text           00000004 _Znwj
08002ea8 g       .text           0000003c _ZTVN5codal10MessageBusE
08000b84 g     F .text           00000018 _ZN5codal9idle_taskEv
08001ac0 g     F .text           00000004 _ZN10DynamicPwm10getPinNameEv
080021a0 g     F .text           00000024 .hidden __floatunsisf
080012a8 g     F .text           00000008 _ZThn8_N5codal10MessageBusD0Ev
08001da0 gw    F .text           00000002 tim8_cc_isr
08001690 g     F .text           0000004c _ZN5codal13STM32BluePillC1Ev
08000f98 g     F .text           00000016 _ZN5codal10MessageBus9elementAtEi
0800128c g     F .text           00000008 _ZThn8_N5codal10MessageBusD1Ev
08001a9c g     F .text           00000002 _ZN5codal3_cm5Timer9triggerInEy
080009bc g     F .text           00000014 _ZN5codal8scheduleEv
0800162c g     F .text           00000002 _Z25stm32bluepill_dmesg_flushv
08001818 g     F .text           0000002c _ZN5codal3_cm3Pin13setServoValueEiii
08001da0 gw    F .text           00000002 tamper_isr
08001ab4 g     F .text           00000006 _ZN10DynamicPwmC2E7PinName
080027f0 gw    F .text           00000002 .hidden __aeabi_idiv0
080030ac         .init_array             00000000 __init_array_start
08001e94 g     F .text           00000006 gpio_clear
08000330 g     F .text           000000dc _ZN5codal14CodalComponent12addComponentEv
08001da0 gw    F .text           00000002 eth_wkup_isr
08001764 g     F .text           00000010 enable_debug
08001500 g     F .text           00000046 _ZN5codal10MessageBus4idleENS_5EventE
08001580 g     F .text           0000001e _ZN5codal5EventC2EttNS_15EventLaunchModeE
08000f88 gw    F .text           00000010 _ZN5codal10EventModel4sendENS_5EventE
08002da0 g     F .text           00000044 os_task_init
08001478 g     F .text           0000006e _ZN5codal10MessageBus10queueEventERNS_5EventE
08002e28 g     F .text           00000010 strlen
08001b34 g     F .text           00000004 tcb_configure_lr
20005000         .bss            00000000 _stack
08001e90 g     F .text           00000004 gpio_set
08000d54 g     F .text           00000096 _ZN5codal8Listener5queueENS_5EventE
08001da0 gw    F .text           00000002 bus_fault_handler
08001da0 gw    F .text           00000002 wwdg_isr
08001da0 gw    F .text           00000002 dma1_channel3_isr
08000df8 g     F .text           0000000a _ZN5codal5Timer16enableInterruptsEv
08000f14 g     F .text           00000002 _ZN5codal5TimerD1Ev
08002d70 g     F .text           00000024 os_msgQ_init
080030ac         .preinit_array          00000000 __preinit_array_start
08001da0 gw    F .text           00000002 spi3_isr
08002df0 g     F .text           0000000a __aeabi_atexit
08002040 g     F .text           00000160 .hidden __addsf3
080018a4 g     F .text           00000054 _ZN5codal3_cm3Pin9isTouchedEv
08001da0 gw    F .text           00000002 tim2_isr
08000cb4 g     F .text           00000040 free
08001da0 gw    F .text           00000002 exti0_isr
08002958 g     F .text           00000006 _ZN5codal3I2C5startEv

```