# codal-libopencm3
Codal framework ported to libopencm3 to support MakeCode on STM32 Blue Pill

Used by MakeCode on STM32 Blue Pill:

https://github.com/lupyuen/pxt-maker

Generated from these STM32 Blue Pill pin definitions:

https://docs.google.com/spreadsheets/d/1yLciHFyPfhRfwEcG3wfqHDgRFa5OoQYTk63bUmTboa8/edit#gid=1888249278

Based on code from:

https://github.com/lupyuen/send_altitude_cocoos

https://github.com/lancaster-university/codal-stm32

https://github.com/mmoskal/codal-generic-f103re

https://github.com/LabAixBidouille-STM32/codal-stm32-iot-node

## Codal Installation

Original instructions: https://lancaster-university.github.io/codal/

Before using this target you need to configure your platforms with some software.
Codal is also a build system to simplify as much as possible the experience of novice users.  

1. Install `git`, ensure it is available on your platforms path.
2. Install the `arm-none-eabi-*` command line utilities for ARM based devices and/or `avr-gcc`, `avr-binutils`, `avr-libc` for AVR based devices, ensure they are available on your platforms path.
3. Install [CMake](https://cmake.org)(Cross platform make), this is the entirety of the build system.
4. Install `Python 2.7` (if you are unfamiliar with CMake), python scripts are used to simplify the build process.
5. For Windows, install `ninja`: https://github.com/ninja-build/ninja/wiki/Pre-built-Ninja-packages
6. Clone the repository <https://github.com/lancaster-university/codal>

## Building With Codal

- Create a `codal.json` file at the root of the `codal` repository (not `codal-libopencm3`). See below for the content.
- In the root of the `codal` repository (not `codal-libopencm3`) type `python build.py` the `-c` option cleans before building.
- For verbose output, type `export VERBOSE=1` before building
- The bin file `STM32_BLUE_PILL.bin` and hex file `STM32_BLUE_PILL.hex` will be placed at the location specified by `codal.json`, by default this is the root.
- The ELF executable file is generated in `build/STM32_BLUE_PILL`. To dump the file: `objdump -t build/STM32_BLUE_PILL` (For Windows: `arm-none-eabi-objdump -t build/STM32_BLUE_PILL`)
- For troubleshooting, the linker output map is generated in `build/STM32_BLUE_PILL.map`. This is configured in `target.json` and `target-locked.json` in `codal-libopencm3`. 

## Building With Visual Studio Code and PlatformIO

The project may also be built in Visual Studio Code with PlatformIO extension for quick testing.

1. Run these commands to download this repository:

```cmd
git clone --recurse-submodules https://github.com/lupyuen/codal-libopencm3
cd codal-libopencm3
copy patches/codal-core/library.json lib/codal-core/
```

1. On Mac and Linux, change `copy` to `cp`

1. Launch Visual Studio Code and open the workspace file `codal-libopencm3/workspace.code-workspace`

1. Install the PlatformIO extension for Visual Studio Code

1. Click the PlatformIO `Build` command to build.  The main program used is located at `codal-libopencm3/src/main.cpp`.

## Running On Blue Pill

You will need ST Link V2 to flash your Blue Pill with the built executable.  Follow the instructions here to flash and monitor your Blue Pill with PlatformIO:

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

## Exporting Symbols

TODO: `"-Wl,--just-symbols=bootloader.sym"`

```bash
cd build
cmake .. -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=MinSizeRel
make
```

## Codal Build Log

```cmd
Microsoft Windows [Version 10.0.18282.1000]
(c) 2018 Microsoft Corporation. All rights reserved.

C:\tmp>git clone https://github.com/lancaster-university/codal
Cloning into 'codal'...
remote: Enumerating objects: 23, done.
remote: Counting objects: 100% (23/23), done.
remote: Compressing objects: 100% (18/18), done.
remote: Total 2865 (delta 11), reused 12 (delta 5), pack-reused 2842 eceiving objects:  99% (2837/2865), 788.01 KiB | 767.00 KiB/s
Receiving objects: 100% (2865/2865), 1.35 MiB | 767.00 KiB/s, done.
Resolving deltas: 100% (1883/1883), done.
Checking connectivity... done.

C:\tmp>cd codal

C:\tmp\codal>type ..\codal.json
{
    "target": {
        "url": "https://github.com/lupyuen/codal-libopencm3",
        "type": "git",
        "name": "codal-libopencm3",
        "branch": "master"
    }
}

C:\tmp\codal>copy ..\codal.json
        1 file(s) copied.

C:\tmp\codal>python build.py
Creating libraries folder
Cloning into: https://github.com/lupyuen/codal-libopencm3
Cloning into 'codal-libopencm3'...
fatal: no submodule mapping found in .gitmodules for path 'lib/codal-core'
Checking out branch: master
Your branch is up-to-date with 'origin/master'.
Already on 'master'
Set target: codal-libopencm3
Using target-locked.json
Targeting codal-libopencm3
-- The C compiler identification is GNU 7.3.1
-- The CXX compiler identification is GNU 7.3.1
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Detecting C compile features
-- Detecting C compile features - done
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- The ASM compiler identification is GNU
-- Found assembler: C:/Program Files (x86)/GNU Tools Arm Embedded/7 2018-q2-update/bin/arm-none-eabi-gcc.exe
Supressing -Wexpansion-to-defined.
-- Adding library path: (C:/tmp/codal/libraries)
Installing dependencies...
Cloning into: https://github.com/lancaster-university/codal-core
Cloning into 'codal-core'...
Checking out branch: main
error: pathspec 'main' did not match any file(s) known to git.
Using library: codal-libopencm3
CODAL_CORE_DIR: C:/tmp/codal/libraries/codal-libopencm3/../codal-core
COCOOS_DIR: C:/tmp/codal/libraries/codal-libopencm3/lib/cocoOS_5.0.3
BME280_DIR: C:/tmp/codal/libraries/codal-libopencm3/lib/BME280
BOOST_DIR: C:/tmp/codal/libraries/codal-libopencm3/lib/boost_
STM32_DIR: C:/tmp/codal/libraries/codal-libopencm3/stm32
LIBOPENCM3_DIR: C:/tmp/codal/libraries/codal-libopencm3/lib/libopencm3
Cloning into 'libopencm3'...
CODAL_LIBOPENCM3_DIR: C:/tmp/codal/libraries/codal-libopencm3/src
TOP_LEVEL_INCLUDE_DIRS: C:/tmp/codal/libraries/codal-libopencm3/src
TOP_LEVEL_SOURCE_FILES: C:/tmp/codal/libraries/codal-libopencm3/src/sensor_display.cpp
CODAL_CORE_INC: C:/tmp/codal/libraries/codal-libopencm3/../codal-core/inc/core;C:/tmp/codal/libraries/codal-libopencm3/../codal-core/inc/driver-models;C:/tmp/codal/libraries/codal-libopencm3/../codal-core/inc/drivers;C:/tmp/codal/libraries/codal-libopencm3/../codal-core/inc/streams;C:/tmp/codal/libraries/codal-libopencm3/../codal-core/inc/types
Using library: codal-core
-- Configuring done
-- Generating done
-- Build files have been written to: C:/tmp/codal/build
[9/147] Building C object libraries/codal-libopencm3/CMakeFiles/cocoos.dir/lib/cocoOS_5.0.3/src/os_msgqueue.c.obj
../libraries/codal-libopencm3/lib/cocoOS_5.0.3/src/os_msgqueue.c: In function 'queue_push':
../libraries/codal-libopencm3/lib/cocoOS_5.0.3/src/os_msgqueue.c:160:28: warning: cast from pointer to integer of different size [-Wpointer-to-int-cast]
  uint8_t *dst = (uint8_t*)((Mem_t)queue->list + head * msgSz);
                            ^
../libraries/codal-libopencm3/lib/cocoOS_5.0.3/src/os_msgqueue.c:160:17: warning: cast to pointer from integer of different size [-Wint-to-pointer-cast]
  uint8_t *dst = (uint8_t*)((Mem_t)queue->list + head * msgSz);
                 ^
../libraries/codal-libopencm3/lib/cocoOS_5.0.3/src/os_msgqueue.c: In function 'os_msg_receive':
../libraries/codal-libopencm3/lib/cocoOS_5.0.3/src/os_msgqueue.c:199:27: warning: cast from pointer to integer of different size [-Wpointer-to-int-cast]
         src = (uint8_t*)( (Mem_t)q->list + tail * msgSz );
                           ^
../libraries/codal-libopencm3/lib/cocoOS_5.0.3/src/os_msgqueue.c:199:15: warning: cast to pointer from integer of different size [-Wint-to-pointer-cast]
         src = (uint8_t*)( (Mem_t)q->list + tail * msgSz );
               ^
../libraries/codal-libopencm3/lib/cocoOS_5.0.3/src/os_msgqueue.c:221:31: warning: cast from pointer to integer of different size [-Wpointer-to-int-cast]
             dst = (uint8_t*)( (Mem_t)q->list + q->head * msgSz );
                               ^
../libraries/codal-libopencm3/lib/cocoOS_5.0.3/src/os_msgqueue.c:221:19: warning: cast to pointer from integer of different size [-Wint-to-pointer-cast]
             dst = (uint8_t*)( (Mem_t)q->list + q->head * msgSz );
                   ^
../libraries/codal-libopencm3/lib/cocoOS_5.0.3/src/os_msgqueue.c: In function 'os_msgQ_tick':
../libraries/codal-libopencm3/lib/cocoOS_5.0.3/src/os_msgqueue.c:255:26: warning: cast from pointer to integer of different size [-Wpointer-to-int-cast]
         pMsg = (Msg_t*)( (Mem_t)q->list + nextMessage * msgSz );
                          ^
../libraries/codal-libopencm3/lib/cocoOS_5.0.3/src/os_msgqueue.c:255:16: warning: cast to pointer from integer of different size [-Wint-to-pointer-cast]
         pMsg = (Msg_t*)( (Mem_t)q->list + nextMessage * msgSz );
                ^
../libraries/codal-libopencm3/lib/cocoOS_5.0.3/src/os_msgqueue.c: In function 'MsgQAllDelayed':
../libraries/codal-libopencm3/lib/cocoOS_5.0.3/src/os_msgqueue.c:284:26: warning: cast from pointer to integer of different size [-Wpointer-to-int-cast]
         pMsg = (Msg_t*)( (Mem_t)q->list + (nextMessage * msgSz) );
                          ^
../libraries/codal-libopencm3/lib/cocoOS_5.0.3/src/os_msgqueue.c:284:16: warning: cast to pointer from integer of different size [-Wint-to-pointer-cast]
         pMsg = (Msg_t*)( (Mem_t)q->list + (nextMessage * msgSz) );
                ^
[44/147] Building CXX object libraries/codal-libopencm3/CMakeFiles/bme280.dir/lib/BME280/src/BME280.cpp.obj
../libraries/codal-libopencm3/lib/BME280/src/BME280.cpp: In member function 'bool BME280::WriteSettings()':
../libraries/codal-libopencm3/lib/BME280/src/BME280.cpp:98:1: warning: no return statement in function returning non-void [-Wreturn-type]
 }
 ^
[48/147] Building CXX object libraries/codal-libopencm3/CMakeFiles/stm32.dir/stm32/adcint/adcint.cpp.obj
../libraries/codal-libopencm3/stm32/adcint/adcint.cpp: In function 'void demo_task(void*)':
../libraries/codal-libopencm3/stm32/adcint/adcint.cpp:20:6: warning: unused variable 'vref' [-Wunused-variable]
  int vref = adc_read_scaled_vref();
      ^~~~
../libraries/codal-libopencm3/stm32/adcint/adcint.cpp:21:6: warning: unused variable 'adc0' [-Wunused-variable]
  int adc0 = adc_read(0) * 330 / 4095;
      ^~~~
../libraries/codal-libopencm3/stm32/adcint/adcint.cpp:22:6: warning: unused variable 'adc1' [-Wunused-variable]
  int adc1 = adc_read(1) * 330 / 4095;
      ^~~~
../libraries/codal-libopencm3/stm32/adcint/adcint.cpp:23:6: warning: unused variable 'temp' [-Wunused-variable]
  int temp = adc_read_scaled_temperature();
      ^~~~
../libraries/codal-libopencm3/stm32/adcint/adcint.cpp: At global scope:
../libraries/codal-libopencm3/stm32/adcint/adcint.cpp:18:13: warning: 'void demo_task(void*)' defined but not used [-Wunused-function]
 static void demo_task(void *arg __attribute((unused))) {
             ^~~~~~~~~
[54/147] Building CXX object libraries/codal-libopencm3/CMakeFiles/stm32.dir/stm32/cm/CmSPI.cpp.obj
../libraries/codal-libopencm3/stm32/cm/CmSPI.cpp:85:20: warning: 'int codal::_cm::enable_clock(uint32_t)' defined but not used [-Wunused-function]
         static int enable_clock(uint32_t instance)
                    ^~~~~~~~~~~~
../libraries/codal-libopencm3/stm32/cm/CmSPI.cpp:69:21: warning: 'codal::_cm::instances' defined but not used [-Wunused-variable]
         static SPI *instances[4];
                     ^~~~~~~~~
[145/147] Linking CXX executable STM32_BLUE_PILL
Memory region         Used Size  Region Size  %age Used
             rom:       14464 B        64 KB     22.07%
             ram:        1872 B        20 KB      9.14%
[147/147] converting to bin file.

C:\tmp\codal>
```

## Source Code Download Log

```cmd

C:\>mkdir 181126

C:\>cd 181126

C:\181126>dir
 Volume in drive C is Win2008
 Volume Serial Number is A65F-559C

 Directory of C:\181126

26/11/2018  03:00 PM    <DIR>          .
26/11/2018  03:00 PM    <DIR>          ..
               0 File(s)              0 bytes
               2 Dir(s)  109,298,024,448 bytes free

C:\181126>git clone https://github.com/lupyuen/codal-libopencm3
Cloning into 'codal-libopencm3'...
remote: Enumerating objects: 6, done.
remote: Counting objects: 100% (6/6), done.
remote: Compressing objects: 100% (6/6), done.
remote: Total 2933 (delta 1), reused 2 (delta 0), pack-reused 2927R
 KiB/s
Receiving objects: 100% (2933/2933), 2.37 MiB | 915.00 KiB/s, done.
Resolving deltas: 100% (1455/1455), done.
Checking connectivity... done.
Checking out files: 100% (2289/2289), done.

C:\181126>dir
 Volume in drive C is Win2008
 Volume Serial Number is A65F-559C

 Directory of C:\181126

26/11/2018  03:01 PM    <DIR>          .
26/11/2018  03:01 PM    <DIR>          ..
26/11/2018  03:01 PM    <DIR>          codal-libopencm3
               0 File(s)              0 bytes
               3 Dir(s)  109,275,754,496 bytes free

C:\181126>cd codal-libopencm3

C:\181126\codal-libopencm3>dir
 Volume in drive C is Win2008
 Volume Serial Number is A65F-559C

 Directory of C:\181126\codal-libopencm3

26/11/2018  03:01 PM    <DIR>          .
26/11/2018  03:01 PM    <DIR>          ..
26/11/2018  03:01 PM               221 .gitignore
26/11/2018  03:01 PM             1,620 .travis.yml
26/11/2018  03:01 PM    <DIR>          .vscode
26/11/2018  03:01 PM    <DIR>          asm
26/11/2018  03:01 PM             9,593 CMakeLists.txt
26/11/2018  03:01 PM    <DIR>          docs
26/11/2018  03:01 PM    <DIR>          ld
26/11/2018  03:01 PM    <DIR>          lib
26/11/2018  03:01 PM             1,098 LICENSE
26/11/2018  03:01 PM    <DIR>          mbed
26/11/2018  03:01 PM    <DIR>          patches
26/11/2018  03:01 PM             4,514 platformio.ini
26/11/2018  03:01 PM            23,794 README 2.md
26/11/2018  03:01 PM            93,610 README.md
26/11/2018  03:01 PM    <DIR>          samples
26/11/2018  03:01 PM    <DIR>          scripts
26/11/2018  03:01 PM    <DIR>          source
26/11/2018  03:01 PM    <DIR>          src
26/11/2018  03:01 PM    <DIR>          stm32
26/11/2018  03:01 PM             3,139 target-locked.json
26/11/2018  03:01 PM             3,139 target.json
26/11/2018  03:01 PM             4,428 workspace.code-workspace
              10 File(s)        145,156 bytes
              14 Dir(s)  109,275,426,816 bytes free

C:\181126\codal-libopencm3>cd lib

C:\181126\codal-libopencm3\lib>dir
 Volume in drive C is Win2008
 Volume Serial Number is A65F-559C

 Directory of C:\181126\codal-libopencm3\lib

26/11/2018  03:01 PM    <DIR>          .
26/11/2018  03:01 PM    <DIR>          ..
26/11/2018  03:01 PM    <DIR>          BME280
26/11/2018  03:01 PM    <DIR>          boost_assert
26/11/2018  03:01 PM    <DIR>          boost_config
26/11/2018  03:01 PM    <DIR>          boost_core
26/11/2018  03:01 PM    <DIR>          boost_detail
26/11/2018  03:01 PM    <DIR>          boost_iterator
26/11/2018  03:01 PM    <DIR>          boost_lockfree
26/11/2018  03:01 PM    <DIR>          boost_mpl
26/11/2018  03:01 PM    <DIR>          boost_parameter
26/11/2018  03:01 PM    <DIR>          boost_predef
26/11/2018  03:01 PM    <DIR>          boost_preprocessor
26/11/2018  03:01 PM    <DIR>          boost_static_assert
26/11/2018  03:01 PM    <DIR>          boost_type_traits
26/11/2018  03:01 PM    <DIR>          boost_utility
26/11/2018  03:01 PM    <DIR>          cocoOS_5.0.3
26/11/2018  03:01 PM    <DIR>          codal-core
26/11/2018  03:01 PM    <DIR>          libopencm3
26/11/2018  03:01 PM             1,045 readme.txt
               1 File(s)          1,045 bytes
              19 Dir(s)  109,275,164,672 bytes free

C:\181126\codal-libopencm3\lib>git clone https://github.com/lancaster-university/codal-core
Cloning into 'codal-core'...
remote: Enumerating objects: 172, done.
remote: Counting objects: 100% (172/172), done.
remote: Compressing objects: 100% (110/110), done.
remote: Total 4109 (delta 110), reused 110 (delta 62), pack-reused 3937 eceiving objects:  99% (4068/4109), 512.01 KiB | 502.00 KiB/s
Receiving objects: 100% (4109/4109), 953.04 KiB | 502.00 KiB/s, done.
Resolving deltas: 100% (2994/2994), done.
Checking connectivity... done.

C:\181126\codal-libopencm3\lib>dir
 Volume in drive C is Win2008
 Volume Serial Number is A65F-559C

 Directory of C:\181126\codal-libopencm3\lib

26/11/2018  03:01 PM    <DIR>          .
26/11/2018  03:01 PM    <DIR>          ..
26/11/2018  03:01 PM    <DIR>          BME280
26/11/2018  03:01 PM    <DIR>          boost_assert
26/11/2018  03:01 PM    <DIR>          boost_config
26/11/2018  03:01 PM    <DIR>          boost_core
26/11/2018  03:01 PM    <DIR>          boost_detail
26/11/2018  03:01 PM    <DIR>          boost_iterator
26/11/2018  03:01 PM    <DIR>          boost_lockfree
26/11/2018  03:01 PM    <DIR>          boost_mpl
26/11/2018  03:01 PM    <DIR>          boost_parameter
26/11/2018  03:01 PM    <DIR>          boost_predef
26/11/2018  03:01 PM    <DIR>          boost_preprocessor
26/11/2018  03:01 PM    <DIR>          boost_static_assert
26/11/2018  03:01 PM    <DIR>          boost_type_traits
26/11/2018  03:01 PM    <DIR>          boost_utility
26/11/2018  03:01 PM    <DIR>          cocoOS_5.0.3
26/11/2018  03:02 PM    <DIR>          codal-core
26/11/2018  03:01 PM    <DIR>          libopencm3
26/11/2018  03:01 PM             1,045 readme.txt
               1 File(s)          1,045 bytes
              19 Dir(s)  109,272,281,088 bytes free

C:\181126\codal-libopencm3\lib>dir codal-core
 Volume in drive C is Win2008
 Volume Serial Number is A65F-559C

 Directory of C:\181126\codal-libopencm3\lib\codal-core

26/11/2018  03:02 PM    <DIR>          .
26/11/2018  03:02 PM    <DIR>          ..
26/11/2018  03:02 PM                20 .gitignore
26/11/2018  03:02 PM               903 CMakeLists.txt
26/11/2018  03:02 PM    <DIR>          inc
26/11/2018  03:02 PM             1,098 LICENSE
26/11/2018  03:02 PM    <DIR>          source
               3 File(s)          2,021 bytes
               4 Dir(s)  109,272,281,088 bytes free

C:\181126\codal-libopencm3\lib>dir codal-core\inc
 Volume in drive C is Win2008
 Volume Serial Number is A65F-559C

 Directory of C:\181126\codal-libopencm3\lib\codal-core\inc

26/11/2018  03:02 PM    <DIR>          .
26/11/2018  03:02 PM    <DIR>          ..
26/11/2018  03:02 PM    <DIR>          core
26/11/2018  03:02 PM    <DIR>          driver-models
26/11/2018  03:02 PM    <DIR>          drivers
26/11/2018  03:02 PM    <DIR>          JACDAC
26/11/2018  03:02 PM    <DIR>          streams
26/11/2018  03:02 PM    <DIR>          types
               0 File(s)              0 bytes
               8 Dir(s)  109,272,281,088 bytes free

C:\181126\codal-libopencm3\lib>cd ..

C:\181126\codal-libopencm3>cd lib

C:\181126\codal-libopencm3\lib>dir
 Volume in drive C is Win2008
 Volume Serial Number is A65F-559C

 Directory of C:\181126\codal-libopencm3\lib

26/11/2018  03:01 PM    <DIR>          .
26/11/2018  03:01 PM    <DIR>          ..
26/11/2018  03:01 PM    <DIR>          BME280
26/11/2018  03:01 PM    <DIR>          boost_assert
26/11/2018  03:01 PM    <DIR>          boost_config
26/11/2018  03:01 PM    <DIR>          boost_core
26/11/2018  03:01 PM    <DIR>          boost_detail
26/11/2018  03:01 PM    <DIR>          boost_iterator
26/11/2018  03:01 PM    <DIR>          boost_lockfree
26/11/2018  03:01 PM    <DIR>          boost_mpl
26/11/2018  03:01 PM    <DIR>          boost_parameter
26/11/2018  03:01 PM    <DIR>          boost_predef
26/11/2018  03:01 PM    <DIR>          boost_preprocessor
26/11/2018  03:01 PM    <DIR>          boost_static_assert
26/11/2018  03:01 PM    <DIR>          boost_type_traits
26/11/2018  03:01 PM    <DIR>          boost_utility
26/11/2018  03:01 PM    <DIR>          cocoOS_5.0.3
26/11/2018  03:02 PM    <DIR>          codal-core
26/11/2018  03:01 PM    <DIR>          libopencm3
26/11/2018  03:01 PM             1,045 readme.txt
               1 File(s)          1,045 bytes
              19 Dir(s)  109,275,754,496 bytes free

C:\181126\codal-libopencm3\lib>git clone https://github.com/libopencm3/libopencm3
Cloning into 'libopencm3'...
remote: Enumerating objects: 61, done.
remote: Counting objects: 100% (61/61), done.
remote: Compressing objects: 100% (36/36), done.
remote: Total 24804 (delta 30), reused 50 (delta 25), pack-reused 24743
Receiving objects: 100% (24804/24804), 5.49 MiB | 1024.00 KiB/s, done.
Resolving deltas: 100% (16222/16222), done.
Checking connectivity... done.

C:\181126\codal-libopencm3\lib>dir libopencm3
 Volume in drive C is Win2008
 Volume Serial Number is A65F-559C

 Directory of C:\181126\codal-libopencm3\lib\libopencm3

26/11/2018  03:02 PM    <DIR>          .
26/11/2018  03:02 PM    <DIR>          ..
26/11/2018  03:02 PM               730 .gitignore
26/11/2018  03:02 PM               289 .travis.yml
26/11/2018  03:02 PM            35,744 COPYING.GPL3
26/11/2018  03:02 PM             7,816 COPYING.LGPL3
26/11/2018  03:02 PM    <DIR>          doc
26/11/2018  03:02 PM             3,590 HACKING
26/11/2018  03:02 PM             3,198 HACKING_COMMON_DOC
26/11/2018  03:02 PM    <DIR>          include
26/11/2018  03:02 PM    <DIR>          ld
26/11/2018  03:02 PM    <DIR>          lib
26/11/2018  03:02 PM               521 locm3.sublime-project
26/11/2018  03:02 PM             3,870 Makefile
26/11/2018  03:02 PM    <DIR>          mk
26/11/2018  03:02 PM             7,026 README.md
26/11/2018  03:02 PM    <DIR>          scripts
26/11/2018  03:02 PM    <DIR>          tests
               9 File(s)         62,784 bytes
               9 Dir(s)  109,260,390,400 bytes free

C:\181126\codal-libopencm3\lib>dir libopencm3\include
 Volume in drive C is Win2008
 Volume Serial Number is A65F-559C

 Directory of C:\181126\codal-libopencm3\lib\libopencm3\include

26/11/2018  03:02 PM    <DIR>          .
26/11/2018  03:02 PM    <DIR>          ..
26/11/2018  03:02 PM    <DIR>          libopencm3
26/11/2018  03:02 PM    <DIR>          libopencmsis
               0 File(s)              0 bytes
               4 Dir(s)  109,260,390,400 bytes free

C:\181126\codal-libopencm3\lib>dir libopencm3\include\libopencm3
 Volume in drive C is Win2008
 Volume Serial Number is A65F-559C

 Directory of C:\181126\codal-libopencm3\lib\libopencm3\include\libopencm3

26/11/2018  03:02 PM    <DIR>          .
26/11/2018  03:02 PM    <DIR>          ..
26/11/2018  03:02 PM    <DIR>          cm3
26/11/2018  03:02 PM    <DIR>          dispatch
26/11/2018  03:02 PM               554 docmain.dox
26/11/2018  03:02 PM    <DIR>          efm32
26/11/2018  03:02 PM    <DIR>          ethernet
26/11/2018  03:02 PM               693 license.dox
26/11/2018  03:02 PM    <DIR>          lm3s
26/11/2018  03:02 PM    <DIR>          lm4f
26/11/2018  03:02 PM    <DIR>          lpc13xx
26/11/2018  03:02 PM    <DIR>          lpc17xx
26/11/2018  03:02 PM    <DIR>          lpc43xx
26/11/2018  03:02 PM    <DIR>          msp432
26/11/2018  03:02 PM    <DIR>          sam
26/11/2018  03:02 PM    <DIR>          stm32
26/11/2018  03:02 PM    <DIR>          usb
26/11/2018  03:02 PM    <DIR>          vf6xx
               2 File(s)          1,247 bytes
              16 Dir(s)  109,260,386,304 bytes free

C:\181126\codal-libopencm3\lib>dir libopencm3\include\libopencm3\stm32
 Volume in drive C is Win2008
 Volume Serial Number is A65F-559C

 Directory of C:\181126\codal-libopencm3\lib\libopencm3\include\libopencm3\stm32

26/11/2018  03:02 PM    <DIR>          .
26/11/2018  03:02 PM    <DIR>          ..
26/11/2018  03:02 PM             1,429 adc.h
26/11/2018  03:02 PM            21,185 can.h
26/11/2018  03:02 PM             1,015 cec.h
26/11/2018  03:02 PM    <DIR>          common
26/11/2018  03:02 PM             1,096 comparator.h
26/11/2018  03:02 PM             1,429 crc.h
26/11/2018  03:02 PM             1,234 crs.h
26/11/2018  03:02 PM             1,163 crypto.h
26/11/2018  03:02 PM             1,360 dac.h
26/11/2018  03:02 PM             2,660 dbgmcu.h
26/11/2018  03:02 PM             1,016 dcmi.h
26/11/2018  03:02 PM             2,055 desig.h
26/11/2018  03:02 PM             1,567 dma.h
26/11/2018  03:02 PM             1,030 dma2d.h
26/11/2018  03:02 PM               947 dsi.h
26/11/2018  03:02 PM             1,571 exti.h
26/11/2018  03:02 PM    <DIR>          f0
26/11/2018  03:02 PM    <DIR>          f1
26/11/2018  03:02 PM    <DIR>          f2
26/11/2018  03:02 PM    <DIR>          f3
26/11/2018  03:02 PM    <DIR>          f4
26/11/2018  03:02 PM    <DIR>          f7
26/11/2018  03:02 PM             1,585 flash.h
26/11/2018  03:02 PM             9,462 fsmc.h
26/11/2018  03:02 PM             1,576 gpio.h
26/11/2018  03:02 PM             1,142 hash.h
26/11/2018  03:02 PM             1,020 hrtim.h
26/11/2018  03:02 PM             1,567 i2c.h
26/11/2018  03:02 PM             1,506 iwdg.h
26/11/2018  03:02 PM    <DIR>          l0
26/11/2018  03:02 PM    <DIR>          l1
26/11/2018  03:02 PM    <DIR>          l4
26/11/2018  03:02 PM             1,021 ltdc.h
26/11/2018  03:02 PM             1,604 memorymap.h
26/11/2018  03:02 PM             1,567 pwr.h
26/11/2018  03:02 PM             1,034 quadspi.h
26/11/2018  03:02 PM             1,567 rcc.h
26/11/2018  03:02 PM             1,289 rng.h
26/11/2018  03:02 PM             1,429 rtc.h
26/11/2018  03:02 PM            14,399 sdio.h
26/11/2018  03:02 PM             1,567 spi.h
26/11/2018  03:02 PM             1,492 st_usbfs.h
26/11/2018  03:02 PM             1,450 syscfg.h
26/11/2018  03:02 PM             1,650 timer.h
26/11/2018  03:02 PM             2,290 tools.h
26/11/2018  03:02 PM             1,015 tsc.h
26/11/2018  03:02 PM             1,585 usart.h
26/11/2018  03:02 PM             2,521 wwdg.h
              38 File(s)         96,095 bytes
              12 Dir(s)  109,260,386,304 bytes free

C:\181126\codal-libopencm3\lib>dir libopencm3\include\libopencm3\stm32\f1
 Volume in drive C is Win2008
 Volume Serial Number is A65F-559C

 Directory of C:\181126\codal-libopencm3\lib\libopencm3\include\libopencm3\stm32\f1

26/11/2018  03:02 PM    <DIR>          .
26/11/2018  03:02 PM    <DIR>          ..
26/11/2018  03:02 PM            15,701 adc.h
26/11/2018  03:02 PM             6,452 bkp.h
26/11/2018  03:02 PM             1,165 crc.h
26/11/2018  03:02 PM             1,088 dac.h
26/11/2018  03:02 PM             1,096 dma.h
26/11/2018  03:02 PM               663 doc-stm32f1.h
26/11/2018  03:02 PM             1,293 exti.h
26/11/2018  03:02 PM             4,177 flash.h
26/11/2018  03:02 PM            33,414 gpio.h
26/11/2018  03:02 PM             1,088 i2c.h
26/11/2018  03:02 PM             1,625 irq.json
26/11/2018  03:02 PM             1,173 iwdg.h
26/11/2018  03:02 PM             5,673 memorymap.h
26/11/2018  03:02 PM             1,095 pwr.h
26/11/2018  03:02 PM            25,611 rcc.h
26/11/2018  03:02 PM             5,096 rtc.h
26/11/2018  03:02 PM             1,087 spi.h
26/11/2018  03:02 PM               979 st_usbfs.h
26/11/2018  03:02 PM             1,562 timer.h
26/11/2018  03:02 PM             1,101 usart.h
              20 File(s)        111,139 bytes
               2 Dir(s)  109,260,386,304 bytes free

C:\181126\codal-libopencm3\lib>cd ..

C:\181126\codal-libopencm3>
```

## PlatformIO Build Log

```cmd
> Executing task in folder codal-libopencm3: C:\Users\guppy\.platformio\penv\Scripts\platformio.exe run <

Processing bluepill_f103c8 (framework: libopencm3; platform: ststm32; board: bluepill_f103c8)-------------------------------------------------------------------------------------
Verbose mode can be enabled via `-v, --verbose` option
CONFIGURATION: https://docs.platformio.org/page/boards/ststm32/bluepill_f103c8.html
PLATFORM: ST STM32 > BluePill F103C8
HARDWARE: STM32F103C8T6 72MHz 20KB RAM (64KB Flash)
DEBUG: CURRENT(stlink) EXTERNAL(blackmagic, jlink, stlink)
Library Dependency Finder -> http://bit.ly/configure-pio-ldf
LDF MODES: FINDER(chain+) COMPATIBILITY(off)
Collected 27 compatible libraries
Scanning dependencies...
Dependency Graph
|-- <boost_utility>
|   |-- <boost_preprocessor>
|   |-- <boost_type_traits>
|   |   |-- <boost_config>
|   |   |-- <boost_static_assert>
|   |   |   |-- <boost_config>
|   |-- <boost_core>
|   |   |-- <boost_config>
|   |   |-- <boost_assert>
|   |-- <boost_config>
|   |-- <boost_static_assert>
|   |   |-- <boost_config>
|-- <boost_core>
|   |-- <boost_config>
|   |-- <boost_assert>
|-- <boost_preprocessor>
|-- <logger>
|-- <boost_assert>
|-- <boost_config>
|-- <boost_predef>
|-- <i2cint>
|   |-- <bluepill>
|   |   |-- <logger>
|   |   |-- <codal-core>
|   |-- <logger>
|-- <boost_parameter>
|   |-- <boost_preprocessor>
|   |-- <boost_utility>
|   |   |-- <boost_preprocessor>
|   |   |-- <boost_type_traits>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_static_assert>
|   |   |   |   |-- <boost_config>
|   |   |-- <boost_core>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_assert>
|   |   |-- <boost_config>
|   |   |-- <boost_static_assert>
|   |   |   |-- <boost_config>
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
|   |   |-- <boost_static_assert>
|   |   |   |-- <boost_config>
|   |   |-- <boost_core>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_assert>
|   |   |-- <boost_assert>
|   |-- <boost_mpl>
|   |   |-- <boost_preprocessor>
|   |   |-- <boost_config>
|   |   |-- <boost_static_assert>
|   |   |   |-- <boost_config>
|   |   |-- <boost_type_traits>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_static_assert>
|   |   |   |   |-- <boost_config>
|   |   |-- <boost_predef>
|   |   |-- <boost_core>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_assert>
|   |   |-- <boost_utility>
|   |   |   |-- <boost_preprocessor>
|   |   |   |-- <boost_type_traits>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |-- <boost_config>
|   |   |   |-- <boost_core>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_assert>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_static_assert>
|   |   |   |   |-- <boost_config>
|   |-- <boost_static_assert>
|   |   |-- <boost_config>
|   |-- <boost_config>
|   |-- <boost_core>
|   |   |-- <boost_config>
|   |   |-- <boost_assert>
|-- <cm>
|   |-- <logger>
|   |-- <codal-core>
|   |-- <bluepill>
|   |   |-- <logger>
|   |   |-- <codal-core>
|   |-- <cocoOS> 5.0.4
|-- <boost_mpl>
|   |-- <boost_preprocessor>
|   |-- <boost_config>
|   |-- <boost_static_assert>
|   |   |-- <boost_config>
|   |-- <boost_type_traits>
|   |   |-- <boost_config>
|   |   |-- <boost_static_assert>
|   |   |   |-- <boost_config>
|   |-- <boost_predef>
|   |-- <boost_core>
|   |   |-- <boost_config>
|   |   |-- <boost_assert>
|   |-- <boost_utility>
|   |   |-- <boost_preprocessor>
|   |   |-- <boost_type_traits>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_static_assert>
|   |   |   |   |-- <boost_config>
|   |   |-- <boost_core>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_assert>
|   |   |-- <boost_config>
|   |   |-- <boost_static_assert>
|   |   |   |-- <boost_config>
|-- <uartint>
|   |-- <bluepill>
|   |   |-- <logger>
|   |   |-- <codal-core>
|   |-- <boost_lockfree>
|   |   |-- <boost_iterator>
|   |   |   |-- <boost_mpl>
|   |   |   |   |-- <boost_preprocessor>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_type_traits>
|   |   |   |   |   |-- <boost_config>
|   |   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_predef>
|   |   |   |   |-- <boost_core>
|   |   |   |   |   |-- <boost_config>
|   |   |   |   |   |-- <boost_assert>
|   |   |   |   |-- <boost_utility>
|   |   |   |   |   |-- <boost_preprocessor>
|   |   |   |   |   |-- <boost_type_traits>
|   |   |   |   |   |   |-- <boost_config>
|   |   |   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |   |   |-- <boost_config>
|   |   |   |   |   |-- <boost_core>
|   |   |   |   |   |   |-- <boost_config>
|   |   |   |   |   |   |-- <boost_assert>
|   |   |   |   |   |-- <boost_config>
|   |   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |   |-- <boost_config>
|   |   |   |-- <boost_detail>
|   |   |   |   |-- <boost_type_traits>
|   |   |   |   |   |-- <boost_config>
|   |   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_preprocessor>
|   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_core>
|   |   |   |   |   |-- <boost_config>
|   |   |   |   |   |-- <boost_assert>
|   |   |   |   |-- <boost_assert>
|   |   |   |-- <boost_type_traits>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |-- <boost_config>
|   |   |   |-- <boost_core>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_assert>
|   |   |   |-- <boost_preprocessor>
|   |   |   |-- <boost_static_assert>
|   |   |   |   |-- <boost_config>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_assert>
|   |   |   |-- <boost_utility>
|   |   |   |   |-- <boost_preprocessor>
|   |   |   |   |-- <boost_type_traits>
|   |   |   |   |   |-- <boost_config>
|   |   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_core>
|   |   |   |   |   |-- <boost_config>
|   |   |   |   |   |-- <boost_assert>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |-- <boost_config>
|   |   |-- <boost_predef>
|   |   |-- <boost_mpl>
|   |   |   |-- <boost_preprocessor>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_static_assert>
|   |   |   |   |-- <boost_config>
|   |   |   |-- <boost_type_traits>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |-- <boost_config>
|   |   |   |-- <boost_predef>
|   |   |   |-- <boost_core>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_assert>
|   |   |   |-- <boost_utility>
|   |   |   |   |-- <boost_preprocessor>
|   |   |   |   |-- <boost_type_traits>
|   |   |   |   |   |-- <boost_config>
|   |   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_core>
|   |   |   |   |   |-- <boost_config>
|   |   |   |   |   |-- <boost_assert>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |-- <boost_config>
|   |   |-- <boost_type_traits>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_static_assert>
|   |   |   |   |-- <boost_config>
|   |   |-- <boost_preprocessor>
|   |   |-- <boost_parameter>
|   |   |   |-- <boost_preprocessor>
|   |   |   |-- <boost_utility>
|   |   |   |   |-- <boost_preprocessor>
|   |   |   |   |-- <boost_type_traits>
|   |   |   |   |   |-- <boost_config>
|   |   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_core>
|   |   |   |   |   |-- <boost_config>
|   |   |   |   |   |-- <boost_assert>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |-- <boost_config>
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
|   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_core>
|   |   |   |   |   |-- <boost_config>
|   |   |   |   |   |-- <boost_assert>
|   |   |   |   |-- <boost_assert>
|   |   |   |-- <boost_mpl>
|   |   |   |   |-- <boost_preprocessor>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_type_traits>
|   |   |   |   |   |-- <boost_config>
|   |   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_predef>
|   |   |   |   |-- <boost_core>
|   |   |   |   |   |-- <boost_config>
|   |   |   |   |   |-- <boost_assert>
|   |   |   |   |-- <boost_utility>
|   |   |   |   |   |-- <boost_preprocessor>
|   |   |   |   |   |-- <boost_type_traits>
|   |   |   |   |   |   |-- <boost_config>
|   |   |   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |   |   |-- <boost_config>
|   |   |   |   |   |-- <boost_core>
|   |   |   |   |   |   |-- <boost_config>
|   |   |   |   |   |   |-- <boost_assert>
|   |   |   |   |   |-- <boost_config>
|   |   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |   |-- <boost_config>
|   |   |   |-- <boost_static_assert>
|   |   |   |   |-- <boost_config>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_core>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_assert>
|   |   |-- <boost_static_assert>
|   |   |   |-- <boost_config>
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
|   |   |   |-- <boost_core>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_assert>
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
|   |   |   |-- <boost_static_assert>
|   |   |   |   |-- <boost_config>
|   |   |   |-- <boost_core>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_assert>
|   |   |   |-- <boost_assert>
|   |-- <logger>
|-- <bluepill>
|   |-- <logger>
|   |-- <codal-core>
|-- <cocoOS> 5.0.4
|-- <codal-core>
|-- <adcint>
|-- <boost_lockfree>
|   |-- <boost_iterator>
|   |   |-- <boost_mpl>
|   |   |   |-- <boost_preprocessor>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_static_assert>
|   |   |   |   |-- <boost_config>
|   |   |   |-- <boost_type_traits>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |-- <boost_config>
|   |   |   |-- <boost_predef>
|   |   |   |-- <boost_core>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_assert>
|   |   |   |-- <boost_utility>
|   |   |   |   |-- <boost_preprocessor>
|   |   |   |   |-- <boost_type_traits>
|   |   |   |   |   |-- <boost_config>
|   |   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_core>
|   |   |   |   |   |-- <boost_config>
|   |   |   |   |   |-- <boost_assert>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |-- <boost_config>
|   |   |-- <boost_detail>
|   |   |   |-- <boost_type_traits>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |-- <boost_config>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_preprocessor>
|   |   |   |-- <boost_static_assert>
|   |   |   |   |-- <boost_config>
|   |   |   |-- <boost_core>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_assert>
|   |   |   |-- <boost_assert>
|   |   |-- <boost_type_traits>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_static_assert>
|   |   |   |   |-- <boost_config>
|   |   |-- <boost_core>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_assert>
|   |   |-- <boost_preprocessor>
|   |   |-- <boost_static_assert>
|   |   |   |-- <boost_config>
|   |   |-- <boost_config>
|   |   |-- <boost_assert>
|   |   |-- <boost_utility>
|   |   |   |-- <boost_preprocessor>
|   |   |   |-- <boost_type_traits>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |-- <boost_config>
|   |   |   |-- <boost_core>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_assert>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_static_assert>
|   |   |   |   |-- <boost_config>
|   |-- <boost_predef>
|   |-- <boost_mpl>
|   |   |-- <boost_preprocessor>
|   |   |-- <boost_config>
|   |   |-- <boost_static_assert>
|   |   |   |-- <boost_config>
|   |   |-- <boost_type_traits>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_static_assert>
|   |   |   |   |-- <boost_config>
|   |   |-- <boost_predef>
|   |   |-- <boost_core>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_assert>
|   |   |-- <boost_utility>
|   |   |   |-- <boost_preprocessor>
|   |   |   |-- <boost_type_traits>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |-- <boost_config>
|   |   |   |-- <boost_core>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_assert>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_static_assert>
|   |   |   |   |-- <boost_config>
|   |-- <boost_type_traits>
|   |   |-- <boost_config>
|   |   |-- <boost_static_assert>
|   |   |   |-- <boost_config>
|   |-- <boost_preprocessor>
|   |-- <boost_parameter>
|   |   |-- <boost_preprocessor>
|   |   |-- <boost_utility>
|   |   |   |-- <boost_preprocessor>
|   |   |   |-- <boost_type_traits>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |-- <boost_config>
|   |   |   |-- <boost_core>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_assert>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_static_assert>
|   |   |   |   |-- <boost_config>
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
|   |   |   |-- <boost_static_assert>
|   |   |   |   |-- <boost_config>
|   |   |   |-- <boost_core>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_assert>
|   |   |   |-- <boost_assert>
|   |   |-- <boost_mpl>
|   |   |   |-- <boost_preprocessor>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_static_assert>
|   |   |   |   |-- <boost_config>
|   |   |   |-- <boost_type_traits>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |-- <boost_config>
|   |   |   |-- <boost_predef>
|   |   |   |-- <boost_core>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_assert>
|   |   |   |-- <boost_utility>
|   |   |   |   |-- <boost_preprocessor>
|   |   |   |   |-- <boost_type_traits>
|   |   |   |   |   |-- <boost_config>
|   |   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_core>
|   |   |   |   |   |-- <boost_config>
|   |   |   |   |   |-- <boost_assert>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |-- <boost_config>
|   |   |-- <boost_static_assert>
|   |   |   |-- <boost_config>
|   |   |-- <boost_config>
|   |   |-- <boost_core>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_assert>
|   |-- <boost_static_assert>
|   |   |-- <boost_config>
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
|   |   |-- <boost_core>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_assert>
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
|   |   |-- <boost_static_assert>
|   |   |   |-- <boost_config>
|   |   |-- <boost_core>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_assert>
|   |   |-- <boost_assert>
|-- <spiint>
|   |-- <logger>
|   |-- <simulator>
|   |   |-- <logger>
|   |   |-- <cocoOS> 5.0.4
|   |   |-- <bluepill>
|   |   |   |-- <logger>
|   |   |   |-- <codal-core>
|   |-- <cocoOS> 5.0.4
|   |-- <bluepill>
|   |   |-- <logger>
|   |   |-- <codal-core>
|-- <boost_static_assert>
|   |-- <boost_config>
|-- <BME280> 2.3.0
|   |-- <cocoOS> 5.0.4
|   |-- <i2cint>
|   |   |-- <bluepill>
|   |   |   |-- <logger>
|   |   |   |-- <codal-core>
|   |   |-- <logger>
|   |-- <logger>
|   |-- <spiint>
|   |   |-- <logger>
|   |   |-- <simulator>
|   |   |   |-- <logger>
|   |   |   |-- <cocoOS> 5.0.4
|   |   |   |-- <bluepill>
|   |   |   |   |-- <logger>
|   |   |   |   |-- <codal-core>
|   |   |-- <cocoOS> 5.0.4
|   |   |-- <bluepill>
|   |   |   |-- <logger>
|   |   |   |-- <codal-core>
|   |-- <porting>
|   |   |-- <spiint>
|   |   |   |-- <logger>
|   |   |   |-- <simulator>
|   |   |   |   |-- <logger>
|   |   |   |   |-- <cocoOS> 5.0.4
|   |   |   |   |-- <bluepill>
|   |   |   |   |   |-- <logger>
|   |   |   |   |   |-- <codal-core>
|   |   |   |-- <cocoOS> 5.0.4
|   |   |   |-- <bluepill>
|   |   |   |   |-- <logger>
|   |   |   |   |-- <codal-core>
|   |   |-- <i2cint>
|   |   |   |-- <bluepill>
|   |   |   |   |-- <logger>
|   |   |   |   |-- <codal-core>
|   |   |   |-- <logger>
|   |   |-- <cocoOS> 5.0.4
|   |   |-- <bluepill>
|   |   |   |-- <logger>
|   |   |   |-- <codal-core>
|   |-- <codal-core>
|   |-- <cm>
|   |   |-- <logger>
|   |   |-- <codal-core>
|   |   |-- <bluepill>
|   |   |   |-- <logger>
|   |   |   |-- <codal-core>
|   |   |-- <cocoOS> 5.0.4
|-- <boost_type_traits>
|   |-- <boost_config>
|   |-- <boost_static_assert>
|   |   |-- <boost_config>
|-- <simulator>
|   |-- <logger>
|   |-- <cocoOS> 5.0.4
|   |-- <bluepill>
|   |   |-- <logger>
|   |   |-- <codal-core>
|-- <boost_iterator>
|   |-- <boost_mpl>
|   |   |-- <boost_preprocessor>
|   |   |-- <boost_config>
|   |   |-- <boost_static_assert>
|   |   |   |-- <boost_config>
|   |   |-- <boost_type_traits>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_static_assert>
|   |   |   |   |-- <boost_config>
|   |   |-- <boost_predef>
|   |   |-- <boost_core>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_assert>
|   |   |-- <boost_utility>
|   |   |   |-- <boost_preprocessor>
|   |   |   |-- <boost_type_traits>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_static_assert>
|   |   |   |   |   |-- <boost_config>
|   |   |   |-- <boost_core>
|   |   |   |   |-- <boost_config>
|   |   |   |   |-- <boost_assert>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_static_assert>
|   |   |   |   |-- <boost_config>
|   |-- <boost_detail>
|   |   |-- <boost_type_traits>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_static_assert>
|   |   |   |   |-- <boost_config>
|   |   |-- <boost_config>
|   |   |-- <boost_preprocessor>
|   |   |-- <boost_static_assert>
|   |   |   |-- <boost_config>
|   |   |-- <boost_core>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_assert>
|   |   |-- <boost_assert>
|   |-- <boost_type_traits>
|   |   |-- <boost_config>
|   |   |-- <boost_static_assert>
|   |   |   |-- <boost_config>
|   |-- <boost_core>
|   |   |-- <boost_config>
|   |   |-- <boost_assert>
|   |-- <boost_preprocessor>
|   |-- <boost_static_assert>
|   |   |-- <boost_config>
|   |-- <boost_config>
|   |-- <boost_assert>
|   |-- <boost_utility>
|   |   |-- <boost_preprocessor>
|   |   |-- <boost_type_traits>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_static_assert>
|   |   |   |   |-- <boost_config>
|   |   |-- <boost_core>
|   |   |   |-- <boost_config>
|   |   |   |-- <boost_assert>
|   |   |-- <boost_config>
|   |   |-- <boost_static_assert>
|   |   |   |-- <boost_config>
|-- <boost_detail>
|   |-- <boost_type_traits>
|   |   |-- <boost_config>
|   |   |-- <boost_static_assert>
|   |   |   |-- <boost_config>
|   |-- <boost_config>
|   |-- <boost_preprocessor>
|   |-- <boost_static_assert>
|   |   |-- <boost_config>
|   |-- <boost_core>
|   |   |-- <boost_config>
|   |   |-- <boost_assert>
|   |-- <boost_assert>
|-- <porting>
|   |-- <spiint>
|   |   |-- <logger>
|   |   |-- <simulator>
|   |   |   |-- <logger>
|   |   |   |-- <cocoOS> 5.0.4
|   |   |   |-- <bluepill>
|   |   |   |   |-- <logger>
|   |   |   |   |-- <codal-core>
|   |   |-- <cocoOS> 5.0.4
|   |   |-- <bluepill>
|   |   |   |-- <logger>
|   |   |   |-- <codal-core>
|   |-- <i2cint>
|   |   |-- <bluepill>
|   |   |   |-- <logger>
|   |   |   |-- <codal-core>
|   |   |-- <logger>
|   |-- <cocoOS> 5.0.4
|   |-- <bluepill>
|   |   |-- <logger>
|   |   |-- <codal-core>
Compiling .pioenvs\bluepill_f103c8\src\aggregate.o
Compiling .pioenvs\bluepill_f103c8\src\downlink.o
Compiling .pioenvs\bluepill_f103c8\src\main.o
Compiling .pioenvs\bluepill_f103c8\src\sensor.o
Compiling .pioenvs\bluepill_f103c8\src\sensor_display.o
Compiling .pioenvs\bluepill_f103c8\src\uart.o
Compiling .pioenvs\bluepill_f103c8\src\wisol.o
Compiling .pioenvs\bluepill_f103c8\src\wstring.o
Archiving .pioenvs\bluepill_f103c8\liba51\libboost_preprocessor.a
src\sensor.cpp:213:13: warning: 'bool is_valid_event_sensor(Sensor*)' defined but not used [-Wunused-function]
static bool is_valid_event_sensor(Sensor *sensor) {
^~~~~~~~~~~~~~~~~~~~~
Archiving .pioenvs\bluepill_f103c8\lib15a\libboost_config.a
Archiving .pioenvs\bluepill_f103c8\lib7a2\libboost_static_assert.a
Archiving .pioenvs\bluepill_f103c8\lib61b\libboost_type_traits.a
Archiving .pioenvs\bluepill_f103c8\liba46\libboost_assert.a
Archiving .pioenvs\bluepill_f103c8\libb20\libboost_core.a
Archiving .pioenvs\bluepill_f103c8\lib08d\libboost_utility.a
Compiling .pioenvs\bluepill_f103c8\libd9a\logger\logger.o
Archiving .pioenvs\bluepill_f103c8\lib953\libboost_predef.a
Compiling .pioenvs\bluepill_f103c8\lib58f\codal-core\source\JACDAC\JACDAC.o
Compiling .pioenvs\bluepill_f103c8\lib58f\codal-core\source\JACDAC\JDAccelerometerDriver.o
Compiling .pioenvs\bluepill_f103c8\lib58f\codal-core\source\JACDAC\JDBridgeDriver.o
Compiling .pioenvs\bluepill_f103c8\lib58f\codal-core\source\JACDAC\JDDriver.o
Compiling .pioenvs\bluepill_f103c8\lib58f\codal-core\source\JACDAC\JDLogicDriver.o
Compiling .pioenvs\bluepill_f103c8\lib58f\codal-core\source\JACDAC\JDMessageBusDriver.o
Compiling .pioenvs\bluepill_f103c8\lib58f\codal-core\source\JACDAC\JDPinDriver.o
Archiving .pioenvs\bluepill_f103c8\libd9a\liblogger.a
Compiling .pioenvs\bluepill_f103c8\lib58f\codal-core\source\JACDAC\JDProtocol.o
Compiling .pioenvs\bluepill_f103c8\lib58f\codal-core\source\JACDAC\JDReliabilityTester.o
Compiling .pioenvs\bluepill_f103c8\lib58f\codal-core\source\core\CodalCompat.o
Compiling .pioenvs\bluepill_f103c8\lib58f\codal-core\source\core\CodalComponent.o
Compiling .pioenvs\bluepill_f103c8\lib58f\codal-core\source\core\CodalDevice.o
Compiling .pioenvs\bluepill_f103c8\lib58f\codal-core\source\core\CodalDmesg.o
Compiling .pioenvs\bluepill_f103c8\lib58f\codal-core\source\core\CodalFiber.o
Compiling .pioenvs\bluepill_f103c8\lib58f\codal-core\source\core\CodalHeapAllocator.o
Compiling .pioenvs\bluepill_f103c8\lib58f\codal-core\source\core\CodalListener.o
Compiling .pioenvs\bluepill_f103c8\lib58f\codal-core\source\core\CodalUtil.o
Compiling .pioenvs\bluepill_f103c8\lib58f\codal-core\source\core\MemberFunctionCallback.o
Compiling .pioenvs\bluepill_f103c8\lib58f\codal-core\source\driver-models\AbstractButton.o
Compiling .pioenvs\bluepill_f103c8\lib58f\codal-core\source\driver-models\Accelerometer.o
Compiling .pioenvs\bluepill_f103c8\lib58f\codal-core\source\driver-models\CodalUSB.o
Compiling .pioenvs\bluepill_f103c8\lib58f\codal-core\source\driver-models\Compass.o
Compiling .pioenvs\bluepill_f103c8\lib58f\codal-core\source\driver-models\Display.o
Compiling .pioenvs\bluepill_f103c8\lib58f\codal-core\source\driver-models\Gyroscope.o
Compiling .pioenvs\bluepill_f103c8\lib58f\codal-core\source\driver-models\I2C.o
Compiling .pioenvs\bluepill_f103c8\lib58f\codal-core\source\driver-models\SPI.o
Compiling .pioenvs\bluepill_f103c8\lib58f\codal-core\source\driver-models\Sensor.o
Compiling .pioenvs\bluepill_f103c8\lib58f\codal-core\source\driver-models\Timer.o
Compiling .pioenvs\bluepill_f103c8\lib58f\codal-core\source\drivers\AnalogSensor.o
Compiling .pioenvs\bluepill_f103c8\lib58f\codal-core\source\drivers\AnimatedDisplay.o
Compiling .pioenvs\bluepill_f103c8\lib58f\codal-core\source\drivers\AsciiKeyMap.o
Compiling .pioenvs\bluepill_f103c8\lib58f\codal-core\source\drivers\Button.o
Compiling .pioenvs\bluepill_f103c8\lib58f\codal-core\source\drivers\FXOS8700.o
Compiling .pioenvs\bluepill_f103c8\lib58f\codal-core\source\drivers\FXOS8700Accelerometer.o
Compiling .pioenvs\bluepill_f103c8\lib58f\codal-core\source\drivers\FXOS8700Magnetometer.o
Compiling .pioenvs\bluepill_f103c8\lib58f\codal-core\source\drivers\GhostFAT.o
Compiling .pioenvs\bluepill_f103c8\lib58f\codal-core\source\drivers\HID.o
Compiling .pioenvs\bluepill_f103c8\lib58f\codal-core\source\drivers\HIDJoystick.o
Compiling .pioenvs\bluepill_f103c8\lib58f\codal-core\source\drivers\HIDKeyboard.o
Compiling .pioenvs\bluepill_f103c8\lib58f\codal-core\source\drivers\HIDMouse.o
Compiling .pioenvs\bluepill_f103c8\lib58f\codal-core\source\drivers\JackRouter.o
Compiling .pioenvs\bluepill_f103c8\lib58f\codal-core\source\drivers\LEDMatrix.o
Compiling .pioenvs\bluepill_f103c8\lib58f\codal-core\source\drivers\LIS3DH.o
Compiling .pioenvs\bluepill_f103c8\lib58f\codal-core\source\drivers\LinearAnalogSensor.o
Compiling .pioenvs\bluepill_f103c8\lib58f\codal-core\source\drivers\MAG3110.o
Compiling .pioenvs\bluepill_f103c8\lib58f\codal-core\source\drivers\MMA8653.o
Compiling .pioenvs\bluepill_f103c8\lib58f\codal-core\source\drivers\MessageBus.o
Compiling .pioenvs\bluepill_f103c8\lib58f\codal-core\source\drivers\MultiButton.o
Compiling .pioenvs\bluepill_f103c8\lib58f\codal-core\source\drivers\NonLinearAnalogSensor.o
Compiling .pioenvs\bluepill_f103c8\lib58f\codal-core\source\drivers\PearsonHash.o
Compiling .pioenvs\bluepill_f103c8\lib58f\codal-core\source\drivers\ST7735.o
Compiling .pioenvs\bluepill_f103c8\lib58f\codal-core\source\drivers\StandardSPIFlash.o
Compiling .pioenvs\bluepill_f103c8\lib58f\codal-core\source\drivers\TouchButton.o
Compiling .pioenvs\bluepill_f103c8\lib58f\codal-core\source\drivers\TouchSensor.o
Compiling .pioenvs\bluepill_f103c8\lib58f\codal-core\source\drivers\USBMSC.o
Compiling .pioenvs\bluepill_f103c8\lib58f\codal-core\source\streams\DataStream.o
Compiling .pioenvs\bluepill_f103c8\lib58f\codal-core\source\streams\LevelDetector.o
Compiling .pioenvs\bluepill_f103c8\lib58f\codal-core\source\streams\LevelDetectorSPL.o
Compiling .pioenvs\bluepill_f103c8\lib58f\codal-core\source\streams\Mixer.o
Compiling .pioenvs\bluepill_f103c8\lib58f\codal-core\source\streams\StreamNormalizer.o
Compiling .pioenvs\bluepill_f103c8\lib58f\codal-core\source\streams\Synthesizer.o
Compiling .pioenvs\bluepill_f103c8\lib58f\codal-core\source\types\BitmapFont.o
Compiling .pioenvs\bluepill_f103c8\lib58f\codal-core\source\types\CoordinateSystem.o
Compiling .pioenvs\bluepill_f103c8\lib58f\codal-core\source\types\Event.o
Compiling .pioenvs\bluepill_f103c8\lib58f\codal-core\source\types\Image.o
Compiling .pioenvs\bluepill_f103c8\lib58f\codal-core\source\types\ManagedBuffer.o
Compiling .pioenvs\bluepill_f103c8\lib58f\codal-core\source\types\ManagedString.o
Compiling .pioenvs\bluepill_f103c8\lib58f\codal-core\source\types\Matrix4.o
Compiling .pioenvs\bluepill_f103c8\lib58f\codal-core\source\types\RefCounted.o
Compiling .pioenvs\bluepill_f103c8\lib58f\codal-core\source\types\RefCountedInit.o
Compiling .pioenvs\bluepill_f103c8\lib323\bluepill\STM32BluePill.o
Compiling .pioenvs\bluepill_f103c8\lib323\bluepill\STM32BluePillIO.o
Compiling .pioenvs\bluepill_f103c8\lib323\bluepill\STM32BluePillPin.o
Compiling .pioenvs\bluepill_f103c8\lib323\bluepill\bluepill.o
Archiving .pioenvs\bluepill_f103c8\lib58f\libcodal-core.a
Compiling .pioenvs\bluepill_f103c8\lib323\bluepill\led.o
Compiling .pioenvs\bluepill_f103c8\lib323\bluepill\timer.o
Compiling .pioenvs\bluepill_f103c8\lib661\i2cint\i2cint.o
Archiving .pioenvs\bluepill_f103c8\libb40\libboost_detail.a
Archiving .pioenvs\bluepill_f103c8\lib5fb\libboost_mpl.a
Archiving .pioenvs\bluepill_f103c8\libd3a\libboost_parameter.a
Compiling .pioenvs\bluepill_f103c8\lib07d\cocoOS_5.0.3\os_assert.o
Compiling .pioenvs\bluepill_f103c8\lib07d\cocoOS_5.0.3\os_cbk.o
Compiling .pioenvs\bluepill_f103c8\lib07d\cocoOS_5.0.3\os_event.o
Compiling .pioenvs\bluepill_f103c8\lib07d\cocoOS_5.0.3\os_kernel.o
cc1.exe: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Compiling .pioenvs\bluepill_f103c8\lib07d\cocoOS_5.0.3\os_msgqueue.o
cc1.exe: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Compiling .pioenvs\bluepill_f103c8\lib07d\cocoOS_5.0.3\os_sem.o
cc1.exe: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
lib\cocoOS_5.0.3\src\os_msgqueue.c: In function 'queue_push':
lib\cocoOS_5.0.3\src\os_msgqueue.c:160:28: warning: cast from pointer to integer of different size [-Wpointer-to-int-cast]
uint8_t *dst = (uint8_t*)((Mem_t)queue->list + head * msgSz);
^
Alrchiving .pioenvs\bluepill_f103c8\lib323\libbluepill.aib\cocoOS_5.0.3\src\os_msgqueue.c:160:17: warning: cast to pointer from integer of different size [-Wint-to-pointer-cast]
Compiling .pioenvs\bluepill_f103c8\lib07d\cocoOS_5.0.3\os_task.o

uint8_t *dst = (uint8_t*)((Mem_t)queue->list + head * msgSz);Compiling .pioenvs\bluepill_f103c8\liba0a\cm\CmI2C.o

Archiving .pioenvs\bluepill_f103c8\lib661\libi2cint.a
^
lib\cocoOS_5.0.3\src\os_msgqueue.c: In function 'os_msg_receive':
lib\cocoOS_5.0.3\src\os_msgqueue.c:199:27: warning: cast from pointer to integer of different size [-Wpointer-to-int-cast]
src = (uint8_t*)( (Mem_t)q->list + tail * msgSz );
^
lCib\cocoOS_5.0.3\src\os_msgqueue.c:199:15: warning: cast to pointer from integer of different size [-Wint-to-pointer-cast]ompiling .pioenvs\bluepill_f103c8\liba0a\cm\CmPWM.o

src = (uint8_t*)( (Mem_t)q->list + tail * msgSz );
C^ompiling .pioenvs\bluepill_f103c8\liba0a\cm\CmPin.o
lib\cocoOS_5.0.3\src\os_msgqueue.c:221:31: warning: cast from pointer to integer of different size [-Wpointer-to-int-cast]

dst = (uint8_t*)( (Mem_t)q->list + q->head * msgSz );
Compiling .pioenvs\bluepill_f103c8\liba0a\cm\CmSPI.o^
lib\cocoOS_5.0.3\src\os_msgqueue.c:221:19: warning: cast to pointer from integer of different size [-Wint-to-pointer-cast]
dst = (uint8_t*)( (Mem_t)q->list + q->head * msgSz );
^
lib\cocoOS_5.0.3\src\os_msgqueue.c: In function 'os_msgQ_tick':
lib\cocoOS_5.0.3\src\os_msgqueue.c:255:26: warning: cast from pointer to integer of different size [-Wpointer-to-int-cast]
Compiling .pioenvs\bluepill_f103c8\liba0a\cm\CmSingleWireSerial.o

Cpompiling .pioenvs\bluepill_f103c8\liba0a\cm\CmTimer.o
Msg = (Msg_t*)( (Mem_t)q->list + nextMessage * msgSz );
C^ompiling .pioenvs\bluepill_f103c8\liba0a\cm\CmUSB.o

lib\cocoOS_5.0.3\src\os_msgqueue.c:255:16: warning: cast to pointer from integer of different size [-Wint-to-pointer-cast]Compiling .pioenvs\bluepill_f103c8\liba0a\cm\DynamicPwm.o

pMsg = (Msg_t*)( (Mem_t)q->list + nextMessage * msgSz );
^
lib\cocoOS_5.0.3\src\os_msgqueue.c: In function 'MsgQAllDelayed':
lib\cocoOS_5.0.3\src\os_msgqueue.c:284:26: warning: cast from pointer to integer of different size [-Wpointer-to-int-cast]
pMsg = (Msg_t*)( (Mem_t)q->list + (nextMessage * msgSz) );
^
lib\cocoOS_5.0.3\src\os_msgqueue.c:284:16: warning: cast to pointer from integer of different size [-Wint-to-pointer-cast]
pMsg = (Msg_t*)( (Mem_t)q->list + (nextMessage * msgSz) );
^
cc1.exe: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
stm32\cm\CmSPI.cpp:85:20: warning: 'int codal::_cm::enable_clock(uint32_t)' defined but not used [-Wunused-function]static int enable_clock(uint32_t instance)
^~~~~~~~~~~~
stm32\cm\CmSPI.cpp:69:21: warning: 'codal::_cm::instances' defined but not used [-Wunused-variable]
static SPI *instances[4];
^~~~~~~~~
Compiling .pioenvs\bluepill_f103c8\liba0a\cm\codal_target_hal.o
Archiving .pioenvs\bluepill_f103c8\lib07d\libcocoOS_5.0.3.a
Compiling .pioenvs\bluepill_f103c8\liba0a\cm\dma.o
Compiling .pioenvs\bluepill_f103c8\liba0a\cm\irq.o
Compiling .pioenvs\bluepill_f103c8\liba0a\cm\pinmap.o
cc1.exe: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Compiling .pioenvs\bluepill_f103c8\liba0a\cm\pinmap_common.o
Compiling .pioenvs\bluepill_f103c8\liba0a\cm\pwmout_api.o
cc1.exe: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Compiling .pioenvs\bluepill_f103c8\liba0a\cm\pwmout_device.o
cc1.exe: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
stm32\cm\irq.c: In function 'SysTick_Handler':
stm32\cm\irq.c:15:5: warning: implicit declaration of function 'HAL_IncTick' [-Wimplicit-function-declaration]
HAL_IncTick();
^~~~~~~~~~~
cc1.exe: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Archiving .pioenvs\bluepill_f103c8\libcae\libboost_iterator.a
cc1.exe: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
In file included from stm32\cm\pinmap.c:32:0:
sAtm32\cm\pin_device.h: In function 'stm_pin_SetAFPin':rchiving .pioenvs\bluepill_f103c8\libb4c\libboost_lockfree.a

stm32\cm\pin_device.h:68:9: warning: implicit declaration of function 'LL_GPIO_SetAFPin_8_15' [-Wimplicit-function-declaration]
LL_GPIO_SetAFPin_8_15(gpio, ll_pin, afnum);
^~~~~~~~~~~~~~~~~~~~~
stm32\cm\pin_device.h:70:9: warning: implicit declaration of function 'LL_GPIO_SetAFPin_0_7' [-Wimplicit-function-declaration]
LL_GPIO_SetAFPin_0_7(gpio, ll_pin, afnum);
^~~~~~~~~~~~~~~~~~~~
Compiling .pioenvs\bluepill_f103c8\libf98\uartint\uartint.o
Archiving .pioenvs\bluepill_f103c8\liba0a\libcm.a
Compiling .pioenvs\bluepill_f103c8\lib68c\adcint\adcint.o
Compiling .pioenvs\bluepill_f103c8\lib092\simulator\simulator.o
Compiling .pioenvs\bluepill_f103c8\lib688\spiint\spiint.o
Compiling .pioenvs\bluepill_f103c8\lib5d1\porting\porting.o
stm32\adcint\adcint.cpp: In function 'void demo_task(void*)':
stm32\adcint\adcint.cpp:20:6: warning: unused variable 'vref' [-Wunused-variable]
int vref = adc_read_scaled_vref();
^~~~
stm32\adcint\adcint.cpp:21:6: warning: unused variable 'adc0' [-Wunused-variable]
int adc0 = adc_read(0) * 330 / 4095;
^~~~
stm32\adcint\adcint.cpp:22:6: warning: unused variable 'adc1' [-Wunused-variable]
int adc1 = adc_read(1) * 330 / 4095;
^~~~
stm32\adcint\adcint.cpp:23:6: warning: unused variable 'temp' [-Wunused-variable]
int temp = adc_read_scaled_temperature();
^~~~
stm32\adcint\adcint.cpp: At global scope:
stm32\adcint\adcint.cpp:18:13: warning: 'void demo_task(void*)' defined but not used [-Wunused-function]
static void demo_task(void *arg __attribute((unused))) {
^~~~~~~~~
Compiling .pioenvs\bluepill_f103c8\lib49b\BME280\BME280.o
Compiling .pioenvs\bluepill_f103c8\lib49b\BME280\BME280I2C.o
Compiling .pioenvs\bluepill_f103c8\lib49b\BME280\BME280Spi.o
Archiving .pioenvs\bluepill_f103c8\lib68c\libadcint.a
Compiling .pioenvs\bluepill_f103c8\lib49b\BME280\EnvironmentCalculations.o
Archiving .pioenvs\bluepill_f103c8\lib092\libsimulator.a
lib\BME280\src\BME280.cpp: In member function 'bool BME280::WriteSettings()':
lib\BME280\src\BME280.cpp:98:1: warning: no return statement in function returning non-void [-Wreturn-type]
}
^
Archiving .pioenvs\bluepill_f103c8\lib5d1\libporting.a
Compiling .pioenvs\bluepill_f103c8\FrameworkLibOpenCM3\lib\stm32\f1\adc.o
Compiling .pioenvs\bluepill_f103c8\FrameworkLibOpenCM3\lib\stm32\common\adc_common_v1.o
Compiling .pioenvs\bluepill_f103c8\FrameworkLibOpenCM3\lib\stm32\can.o
Compiling .pioenvs\bluepill_f103c8\FrameworkLibOpenCM3\lib\stm32\desig.o
cc1.exe: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Archiving .pioenvs\bluepill_f103c8\lib49b\libBME280.a
Compiling .pioenvs\bluepill_f103c8\FrameworkLibOpenCM3\lib\stm32\f1\ethernet.o
cc1.exe: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Compiling .pioenvs\bluepill_f103c8\FrameworkLibOpenCM3\lib\stm32\f1\flash.o
Compiling .pioenvs\bluepill_f103c8\FrameworkLibOpenCM3\lib\stm32\f1\gpio.o
Compiling .pioenvs\bluepill_f103c8\FrameworkLibOpenCM3\lib\stm32\f1\rcc.o
cc1.exe: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Archiving .pioenvs\bluepill_f103c8\lib688\libspiint.a
cc1.exe: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Compiling .pioenvs\bluepill_f103c8\FrameworkLibOpenCM3\lib\stm32\f1\rtc.o
cc1.exe: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Compiling .pioenvs\bluepill_f103c8\FrameworkLibOpenCM3\lib\stm32\f1\timer.o
Compiling .pioenvs\bluepill_f103c8\FrameworkLibOpenCM3\lib\ethernet\mac.o
cc1.exe: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Compiling .pioenvs\bluepill_f103c8\FrameworkLibOpenCM3\lib\ethernet\mac_stm32fxx7.o
cc1.exe: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Compiling .pioenvs\bluepill_f103c8\FrameworkLibOpenCM3\lib\ethernet\phy.o
Compiling .pioenvs\bluepill_f103c8\FrameworkLibOpenCM3\lib\ethernet\phy_ksz8051mll.o
cc1.exe: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Compiling .pioenvs\bluepill_f103c8\FrameworkLibOpenCM3\lib\stm32\common\crc_common_all.o
cc1.exe: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Compiling .pioenvs\bluepill_f103c8\FrameworkLibOpenCM3\lib\stm32\common\dac_common_all.o
Compiling .pioenvs\bluepill_f103c8\FrameworkLibOpenCM3\lib\stm32\common\dma_common_l1f013.o
cc1.exe: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Compiling .pioenvs\bluepill_f103c8\FrameworkLibOpenCM3\lib\stm32\common\gpio_common_all.o
cc1.exe: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Compiling .pioenvs\bluepill_f103c8\FrameworkLibOpenCM3\lib\stm32\common\i2c_common_all.o
cc1.exe: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
Ccc1.exe: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
ompiling .pioenvs\bluepill_f103c8\FrameworkLibOpenCM3\lib\stm32\common\iwdg_common_all.o
cc1.exe: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for CCompiling .pioenvs\bluepill_f103c8\FrameworkLibOpenCM3\lib\stm32\common\pwr_common_all.o

Compiling .pioenvs\bluepill_f103c8\FrameworkLibOpenCM3\lib\stm32\common\spi_common_all.o
cc1.exe: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Compiling .pioenvs\bluepill_f103c8\FrameworkLibOpenCM3\lib\stm32\common\spi_common_l1f124.oc
c1.exe: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for CCompiling .pioenvs\bluepill_f103c8\FrameworkLibOpenCM3\lib\stm32\common\timer_common_all.o

cc1.exe: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Compiling .pioenvs\bluepill_f103c8\FrameworkLibOpenCM3\lib\stm32\common\usart_common_all.o
cc1.exe: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Compiling .pioenvs\bluepill_f103c8\FrameworkLibOpenCM3\lib\stm32\common\usart_common_f124.o
Compiling .pioenvs\bluepill_f103c8\FrameworkLibOpenCM3\lib\stm32\common\rcc_common_all.o
cc1.exe: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Compiling .pioenvs\bluepill_f103c8\FrameworkLibOpenCM3\lib\stm32\common\exti_common_all.o
cc1.exe: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for CCompiling .pioenvs\bluepill_f103c8\FrameworkLibOpenCM3\lib\stm32\common\flash_common_f01.o
cc1.exe: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C

Compiling .pioenvs\bluepill_f103c8\FrameworkLibOpenCM3\lib\usb\usb.o
cc1.exe: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for CC
ompiling .pioenvs\bluepill_f103c8\FrameworkLibOpenCM3\lib\usb\usb_control.occ1.exe: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C

Compiling .pioenvs\bluepill_f103c8\FrameworkLibOpenCM3\lib\usb\usb_standard.o
Compiling .pioenvs\bluepill_f103c8\FrameworkLibOpenCM3\lib\usb\usb_msc.o
cc1.exe: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Compiling .pioenvs\bluepill_f103c8\FrameworkLibOpenCM3\lib\usb\usb_fx07_common.o
cc1.exe: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Compiling .pioenvs\bluepill_f103c8\FrameworkLibOpenCM3\lib\usb\usb_f107.o
cc1.exe: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for CCompiling .pioenvs\bluepill_f103c8\FrameworkLibOpenCM3\lib\stm32\common\st_usbfs_core.o

cc1.exe: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Compiling .pioenvs\bluepill_f103c8\FrameworkLibOpenCM3\lib\stm32\st_usbfs_v1.o
cc1.exe: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Compiling .pioenvs\bluepill_f103c8\FrameworkLibOpenCM3\lib\cm3\vector.o
Compiling .pioenvs\bluepill_f103c8\FrameworkLibOpenCM3\lib\cm3\systick.o
Compiling .pioenvs\bluepill_f103c8\FrameworkLibOpenCM3\lib\cm3\scb.o
cc1.exe: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Compiling .pioenvs\bluepill_f103c8\FrameworkLibOpenCM3\lib\cm3\nvic.o
cc1.exe: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Compiling .pioenvs\bluepill_f103c8\FrameworkLibOpenCM3\lib\cm3\assert.o
Archiving .pioenvs\bluepill_f103c8\libf98\libuartint.a
cc1.exe: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Compiling .pioenvs\bluepill_f103c8\FrameworkLibOpenCM3\lib\cm3\sync.o
cc1.exe: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Compiling .pioenvs\bluepill_f103c8\FrameworkLibOpenCM3\lib\cm3\dwt.o
cc1.exe: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-rtti' is valid for C++/ObjC++ but not for C
cc1.exe: warning: command line option '-fno-threadsafe-statics' is valid for C++/ObjC++ but not for C
Archiving .pioenvs\bluepill_f103c8\libFrameworkLibOpenCM3.a
Linking .pioenvs\bluepill_f103c8\firmware.elf
Memory region         Used Size  Region Size  %age Used
rom:       16568 B        64 KB     25.28%
ram:        3320 B        20 KB     16.21%
Building .pioenvs\bluepill_f103c8\firmware.bin
Checking size .pioenvs\bluepill_f103c8\firmware.elf
Memory Usage -> http://bit.ly/pio-memory-usage
DATA:    [==        ]  16.2% (used 3316 bytes from 20480 bytes)
PROGRAM: [===       ]  25.3% (used 16556 bytes from 65536 bytes)
========================================== [SUCCESS] Took 184.68 seconds ==========================================

Terminal will be reused by tasks, press any key to close it.

```

## Dump of ELF Executable

```cmd
C:\tmp\codal>arm-none-eabi-objdump -t build/STM32_BLUE_PILL

build/STM32_BLUE_PILL:     file format elf32-littlearm

SYMBOL TABLE:
08000000 l    d  .text  00000000 .text
08003850 l    d  .init  00000000 .init
08003854 l    d  .fini  00000000 .fini
08003858 l    d  .preinit_array 00000000 .preinit_array
08003858 l    d  .init_array    00000000 .init_array
08003860 l    d  .fini_array    00000000 .fini_array
08003864 l    d  .ARM.exidx     00000000 .ARM.exidx
20000000 l    d  .data  00000000 .data
20000018 l    d  .bss   00000000 .bss
00000000 l    d  .ARM.attributes        00000000 .ARM.attributes
00000000 l    d  .comment       00000000 .comment
00000000 l    d  .debug_info    00000000 .debug_info
00000000 l    d  .debug_abbrev  00000000 .debug_abbrev
00000000 l    d  .debug_loc     00000000 .debug_loc
00000000 l    d  .debug_aranges 00000000 .debug_aranges
00000000 l    d  .debug_ranges  00000000 .debug_ranges
00000000 l    d  .debug_line    00000000 .debug_line
00000000 l    d  .debug_str     00000000 .debug_str
00000000 l    d  .debug_frame   00000000 .debug_frame
00000000 l    df *ABS*  00000000 vector.c
00000000 l    df *ABS*  00000000 crtstuff.c
08000150 l     F .text  00000000 __do_global_dtors_aux
200000e8 l       .bss   00000001 completed.8655
08003860 l     O .fini_array    00000000 __do_global_dtors_aux_fini_array_entry
08000174 l     F .text  00000000 frame_dummy
200000ec l       .bss   00000018 object.8660
08003858 l     O .init_array    00000000 __frame_dummy_init_array_entry
00000000 l    df *ABS*  00000000 main.cpp
080001d8 l     F .text  00000024 _GLOBAL__sub_I_bluepill
00000000 l    df *ABS*  00000000 CodalComponent.cpp
00000000 l    df *ABS*  00000000 CodalFiber.cpp
08000444 l     F .text  0000002e _ZN5codal13dequeue_fiberEPNS_5FiberE.part.0
08000644 l     F .text  00000034 _ZL10handle_fobv
08000888 l     F .text  00000170 _ZN5codal8scheduleEv.part.3
08000a58 l     F .text  00000048 _ZN5codal13release_fiberEv.part.4
200001f8 l     O .bss   00000004 _ZN5codalL10messageBusE
200001fc l     O .bss   00000004 _ZN5codalL10sleepQueueE
20000741 l     O .bss   00000001 _ZN5codalL11fiber_flagsE
20000200 l     O .bss   00000004 _ZN5codalL11forkedFiberE
20000204 l     O .bss   00000004 _ZN5codalL8runQueueE
20000208 l     O .bss   00000004 _ZN5codalL9fiberPoolE
2000020c l     O .bss   00000004 _ZN5codalL9idleFiberE
20000210 l     O .bss   00000004 _ZN5codalL9waitQueueE
00000000 l    df *ABS*  00000000 CodalHeapAllocator.cpp
20000742 l     O .bss   00000001 _ZZ6mallocE11initialised
00000000 l    df *ABS*  00000000 CodalListener.cpp
00000000 l    df *ABS*  00000000 Timer.cpp
00000000 l    df *ABS*  00000000 MessageBus.cpp
00000000 l    df *ABS*  00000000 Event.cpp
080016a0 l     F .text  00000038 _ZN5codal5EventC2EttNS_15EventLaunchModeE.part.0
080016a0 l     F .text  00000038 _ZN5codal5EventC2EttyNS_15EventLaunchModeE.part.1
00000000 l    df *ABS*  00000000 MemberFunctionCallback.cpp
00000000 l    df *ABS*  00000000 STM32BluePill.cpp
00000000 l    df *ABS*  00000000 STM32BluePillIO.cpp
00000000 l    df *ABS*  00000000 bluepill.cpp
20000744 l     O .bss   00000001 _ZL12debugEnabled
00000000 l    df *ABS*  00000000 led.cpp
00000000 l    df *ABS*  00000000 CmI2C.cpp
00000000 l    df *ABS*  00000000 CmPin.cpp
00000000 l    df *ABS*  00000000 CmTimer.cpp
00000000 l    df *ABS*  00000000 DynamicPwm.cpp
00000000 l    df *ABS*  00000000 codal_target_hal.cpp
08001cb8 l     F .text  00000010 _ZL10timer_tickv
08001cc8 l     F .text  0000002c _Z25stm32bluepill_dmesg_flushv.part.0
20000745 l     O .bss   00000001 _ZL11initialised
20000228 l     O .bss   00000004 _ZL13tick_callback
00000000 l    df *ABS*  00000000 logger.cpp
08001e54 l     F .text  00000040 debug_flush.part.1
08001e94 l     F .text  00000064 _ZL12debug_appendPKcj
08001ef8 l     F .text  00000014 debug_print.part.3
20000746 l     O .bss   00000001 _ZL10logEnabled
2000022c l     O .bss   00000051 _ZL11debugBuffer
00000000 l    df *ABS*  00000000 CortexContextSwitch.s.obj
08002074 l       .text  00000000 store_context_complete
08002086 l       .text  00000000 store_stack_complete
0800207a l       .text  00000000 store_stack
080020a0 l       .text  00000000 restore_stack_complete
08002094 l       .text  00000000 restore_stack
080020f4 l       .text  00000000 store_stack1
00000000 l    df *ABS*  00000000 timer.cpp
20000280 l     O .bss   00000004 _ZL10alarmCount
20000284 l     O .bss   00000004 _ZL8tickFunc
20000288 l     O .bss   00000004 _ZL9tickCount
20000747 l     O .bss   00000001 _ZZ20platform_start_timerE12timerStarted
00000000 l    df *ABS*  00000000 nvic.c
00000000 l    df *ABS*  00000000 exti_common_all.c
00000000 l    df *ABS*  00000000 gpio_common_all.c
00000000 l    df *ABS*  00000000 pwr_common_v1.c
00000000 l    df *ABS*  00000000 rcc_common_all.c
00000000 l    df *ABS*  00000000 gpio.c
00000000 l    df *ABS*  00000000 rcc.c
08002490 l     F .text  00000010 rcc_is_osc_ready.part.0
080024a0 l     F .text  00000010 rcc_osc_on.part.1
00000000 l    df *ABS*  00000000 rtc.c
00000000 l    df *ABS*  00000000 flash_common_all.c
00000000 l    df *ABS*  00000000 _arm_addsubsf3.o
00000000 l    df *ABS*  00000000 _arm_muldivsf3.o
00000000 l    df *ABS*  00000000 _aeabi_uldivmod.o
00000000 l    df *ABS*  00000000 libgcc2.c
00000000 l    df *ABS*  00000000 _dvmd_tls.o
00000000 l    df *ABS*  00000000 cxa_atexit.c
00000000 l    df *ABS*  00000000 CodalDmesg.cpp
2000028c l     O .bss   00000004 _ZL14dmesg_flush_fn
00000000 l    df *ABS*  00000000 I2C.cpp
00000000 l    df *ABS*  00000000 Button.cpp
00000000 l    df *ABS*  00000000 AbstractButton.cpp
00000000 l    df *ABS*  00000000 os_kernel.c
00000000 l    df *ABS*  00000000 os_msgqueue.c
20000694 l     O .bss   00000020 msgQList
20000748 l     O .bss   00000001 nQueues
00000000 l    df *ABS*  00000000 os_sem.c
20000749 l     O .bss   00000001 nSemaphores
00000000 l    df *ABS*  00000000 os_task.c
2000074a l     O .bss   00000001 nTasks
200006b4 l     O .bss   0000008c task_list
00000000 l    df *ABS*  00000000 os_event.c
2000074b l     O .bss   00000001 nEvents
00000000 l    df *ABS*  00000000 atexit_arm.cc
00000000 l    df *ABS*  00000000 memcmp.c
00000000 l    df *ABS*  00000000 memset.c
00000000 l    df *ABS*  00000000 lib_a-strlen.o
00000000 l    df *ABS*  00000000 strncat.c
00000000 l    df *ABS*  00000000 c:/progra~2/gnutoo~1/72018-~1/bin/../lib/gcc/arm-none-eabi/7.3.1/thumb/v7-m/crti.o
08000e1c g     F .text  00000038 _ZN5codal5TimerC1Ev
080013ec g     F .text  00000012 _ZN5codal10MessageBusD0Ev
080010b0 g     F .text  00000030 _ZN5codal27system_timer_event_every_usEytt
08000198  w    F .text  00000002 _ZN5codal14CodalComponent16periodicCallbackEv
08000240  w    F .text  00000038 _ZN5codal13STM32BluePillD1Ev
08003458 g     F .text  00000018 _ZN5codal14AbstractButtonD2Ev
080004ac g     F .text  0000005c _ZN5codal14scheduler_tickENS_5EventE
08003404 g     F .text  00000050 _ZN5codal6ButtonC1ERNS_3PinEt24ButtonEventConfiguration14ButtonPolarityNS_8PullModeE
08001de0 g     F .text  00000004 tcb_configure_stack_base
0800227c  w    F .text  000000e8 reset_handler
08001dc8 g     F .text  00000008 fiber_initial_stack_base
20000743 g     O .bss   00000001 heap_count
080019c0 g     F .text  0000000e _ZN5codal3_cm3Pin8isOutputEv
08000d58 g     F .text  00000014 _ZN5codal8ListenerD2Ev
08001ddc g     F .text  00000004 tcb_configure_sp
08002274  w    F .text  00000002 usart3_isr
08000240  w    F .text  00000038 _ZN5codal13STM32BluePillD2Ev
080021f4 g     F .text  00000040 rtc_isr
08003758 g     O .text  00000018 _ZTVN5codal3_cm5TimerE
08001fbc g     F .text  00000030 _Z11debug_printi
08001408 g     F .text  000000b0 _ZN5codal10MessageBusC2Ev
08002014 g     F .text  00000018 _Z13debug_printlni
08002274  w    F .text  00000002 tim7_isr
08003454 g     F .text  00000004 _ZN5codal14AbstractButton9isPressedEv
08001cb0 g     F .text  00000004 _ZN10DynamicPwm11getPeriodUsEv
080023d0 g     F .text  0000000c exti_reset_request
2000074c g     O .bss   00000001 running_tid
08002274  w    F .text  00000002 adc1_2_isr
0800172c g     F .text  00000028 _ZN5codal14EventQueueItemC2ENS_5EventE
08000f2c g     F .text  00000042 _ZN5codal5Timer4syncEy
08002c3c g     F .text  00000136 .hidden __divsf3
08003664 g     O .text  0000001c _ZTVN5codal13STM32BluePillE
08001de8 g     F .text  00000004 get_current_sp
08002274  w    F .text  00000002 tim1_trg_com_isr
08001944 g     F .text  00000004 _ZN5codal3_cm3I2C12readRegisterEthPhib
080030e8 g     F .text  0000004e _ZN5codal3I2C12readRegisterEthPhib
08000414 g     F .text  00000028 _ZN5codal14CodalComponent15removeComponentEv
0800386c g       .ARM.exidx     00000000 __exidx_end
080016f8 g     F .text  0000001e _ZN5codal5EventC2EttyNS_15EventLaunchModeE
08001b7c g     F .text  0000003a _ZN5codal3_cm3Pin19obtainAnalogChannelEv
08002274  w    F .text  00000002 usb_hp_can_tx_isr
08000e54 g     F .text  000000d8 _ZN5codal5Timer8setEventEyttb
080001a0  w    F .text  00000018 _ZN5codal14CodalComponentD2Ev
080030c8 g     F .text  0000001e _ZN5codal3I2C13writeRegisterEthh
080031cc g     F .text  000000d4 _ZN5codal3I2C4readEtPhib
080032ac  w    F .text  00000006 _ZN5codal3Pin15getDigitalValueEv
08002274  w    F .text  00000002 tim6_isr
0800386c g       .data  00000000 _etext
08002414 g     F .text  0000007c gpio_set_mode
08000474 g     F .text  00000036 _ZN5codal11queue_fiberEPNS_5FiberEPS1_
08002274  w    F .text  00000002 usb_wakeup_isr
08002274 g     F .text  00000002 blocking_handler
2000074d g     O .bss   00000001 running
080018ec g     F .text  00000020 led_setup
080028c4 g     F .text  00000160 .hidden __aeabi_fadd
08002274  w    F .text  00000002 tim5_isr
080028c0 g     F .text  00000164 .hidden __subsf3
08001ff8 g     F .text  0000001c debug_println
08002274  w    F .text  00000002 otg_fs_isr
08001df8 g     F .text  0000005c test_codal
080019ac g     F .text  00000004 _ZN5codal3_cm3Pin14getAnalogValueEv
08002274  w    F .text  00000002 spi1_isr
08000bb4 g     F .text  0000003c _Z18device_create_heapmm
080032c4 g     F .text  00000014 _ZN5codal6ButtonD2Ev
08001af4 g     F .text  00000034 _ZN5codal3_cm3Pin7eventOnEi
08001cb4 g     F .text  00000004 _ZN10DynamicPwm11setPeriodUsEm
08002274  w    F .text  00000002 exti2_isr
08002274  w    F .text  00000002 dma1_channel6_isr
08002278 g     F .text  00000002 null_handler
20000008 g     O .data  00000004 rcc_ahb_frequency
080017f4 g     F .text  0000004c _ZN5codal13STM32BluePillC2Ev
08001754 g     F .text  00000030 _ZN5codal22MemberFunctionCallback4fireENS_5EventE
08002d74 g     F .text  00000000 .hidden __aeabi_uldivmod
08001df0 g     F .text  00000008 tcb_configure_args
08002274  w    F .text  00000002 can_rx1_isr
08003864 g       .fini_array    00000000 __fini_array_end
08002274  w    F .text  00000002 dma1_channel5_isr
08000c80 g     F .text  0000004c malloc
08002274  w    F .text  00000002 dma2_channel5_isr
08003094 g     F .text  0000000c codal_dmesg_set_flush_fn
08000d58 g     F .text  00000014 _ZN5codal8ListenerD1Ev
08002a2c g     F .text  0000001c .hidden __aeabi_i2f
08000bf0 g     F .text  0000008e _Z13device_mallocjR14HeapDefinition
08002840 g     F .text  00000034 rtc_clear_flag
080007c8 g     F .text  00000068 _Z14__create_fibermmmi
080030a0 g     F .text  00000010 codal_dmesg_flush
08002274  w    F .text  00000002 usart1_isr
08000840 g     F .text  00000048 _ZN5codal4idleEv
20000000 g     O .data  00000000 .hidden __dso_handle
08001dec g     F .text  00000004 tcb_get_sp
08001ae0 g     F .text  00000014 _ZN5codal3_cm3Pin17getAnalogPeriodUsEv
08002da4 g     F .text  000002ce .hidden __udivmoddi4
08001940 g     F .text  00000004 _ZN5codal3_cm3I2C4readEtPhib
08000ac0 g     F .text  00000014 _ZN5codal13release_fiberEPv
08001974  w    F .text  00000006 _ZN5codal3Pin8drainPinEv
08002274  w    F .text  00000002 usage_fault_handler
08002274  w    F .text  00000002 tim8_trg_com_isr
08001c70 g     F .text  00000030 _ZN5codal3_cm5TimerC1Ev
08002274  w    F .text  00000002 can2_rx0_isr
080030c0 g     F .text  00000006 _ZN5codal3I2C4readENS_15AcknowledgeTypeE
08002274  w    F .text  00000002 tim1_brk_isr
08001530 g     F .text  000000a0 _ZN5codal10MessageBus7processERNS_5EventEb
080001a0  w    F .text  00000018 _ZN5codal14CodalComponentD1Ev
08003490 g     F .text  0000002c _ZN5codal14AbstractButtonC2Ev
08001f24 g     F .text  00000010 debug_flush
080030b0 g     F .text  00000006 _ZN5codal3I2C12setFrequencyEm
08000a0c g     F .text  0000004c _ZN5codal11fiber_sleepEm
08001640 g     F .text  00000018 _ZN5codal10MessageBus4sendENS_5EventE
080032d8 g     F .text  0000001c _ZN5codal6ButtonD0Ev
08003864 g       .ARM.exidx     00000000 __exidx_start
08002274  w    F .text  00000002 can2_rx1_isr
08003694 g     O .text  0000002c _ZTVN5codal3_cm3I2CE
08003404 g     F .text  00000050 _ZN5codal6ButtonC2ERNS_3PinEt24ButtonEventConfiguration14ButtonPolarityNS_8PullModeE
080018e8 g     F .text  00000004 _ZdlPv
08001b60 g     F .text  0000001c _ZN5codal3_cm3PinC1EihNS_13PinCapabilityE
080023e8 g     F .text  00000010 pwr_disable_backup_domain_write_protect
08002274  w    F .text  00000002 tim1_cc_isr
080019ec g     F .text  00000018 _ZN5codal3_cm3Pin15getAnalogPeriodEv
080023f8 g     F .text  0000001c rcc_periph_clock_enable
08002274  w    F .text  00000002 sdio_isr
08003458 g     F .text  00000018 _ZN5codal14AbstractButtonD1Ev
08003850 g     F .init  00000000 _init
08002274  w    F .text  00000002 eth_isr
080030c0 g     F .text  00000006 _ZN5codal3I2C5writeEh
2000000c g     O .data  00000004 rcc_apb1_frequency
20000740 g     O .bss   00000001 _ZN5codal14CodalComponent13configurationE
08002274  w    F .text  00000002 dma1_channel4_isr
08002274  w    F .text  00000002 tim8_brk_isr
08001070 g     F .text  00000020 _ZN5codal25system_timer_current_timeEv
08002274  w    F .text  00000002 dma2_channel4_5_isr
0800172c g     F .text  00000028 _ZN5codal14EventQueueItemC1ENS_5EventE
200001f4 g     O .bss   00000004 _ZN5codal12currentFiberE
0800106c g     F .text  00000002 _ZN5codal5TimerD2Ev
080028c0 g     F .text  00000164 .hidden __aeabi_fsub
20000220 g     O .bss   00000004 _ZN5codal10EventModel15defaultEventBusE
08000830 g     F .text  00000010 _ZN5codal24scheduler_runqueue_emptyEv
080002c0 g     F .text  00000078 _Z18component_callbackN5codal5EventE
20000750 g       .bss   00000000 _ebss
08001ca8 g     F .text  00000004 _ZN10DynamicPwm5writeEf
080016d8 g     F .text  0000001e _ZN5codal5EventC1EttNS_15EventLaunchModeE
080034bc g     F .text  00000034 os_init
0800213a g       .text  00000000 restore_register_context
08000190  w    F .text  00000008 _ZN5codal14CodalComponent4initEv
08002274  w    F .text  00000002 pvd_isr
080032a0 g     F .text  0000000c _ZN5codal3I2CC2ERNS_3PinES2_
08000d0c g     F .text  0000001c calloc
08001880 g     F .text  0000004c platform_setup
08002a58 g     F .text  0000007c .hidden __floatdisf
0800043c  w    F .text  00000008 _ZN5codal10EventModel6removeEPNS_8ListenerE
08003564 g     F .text  0000000c os_event_init
08001844 g     F .text  0000003c _ZN5codal15STM32BluePillIOC2Ev
08001d58 g     F .text  00000004 target_disable_irq
0800190c g     F .text  00000010 led_on
08000aa0 g     F .text  00000020 _ZN5codal16launch_new_fiberEPFvvES1_
2000074e g     O .bss   00000001 last_running_task
08001360 g     F .text  00000084 _ZN5codal10MessageBusD2Ev
08002278  w    F .text  00000002 sv_call_handler
20000018 g     O .bss   000000d0 bluepill
08002274  w    F .text  00000002 rcc_isr
08000000 g     O .text  00000150 vector_table
0800210a g       .text  00000000 save_register_context
08002274  w    F .text  00000002 flash_isr
0800386c g       *ABS*  00000000 _data_loadaddr
20000750 g       .bss   00000000 end
08002274  w    F .text  00000002 uart4_isr
08002788 g     F .text  0000005c rtc_interrupt_enable
080017a8 g     F .text  0000004c _ZN5codal13STM32BluePill4initEv
08001a60 g     F .text  00000080 _ZN5codal3_cm3Pin15setDigitalValueEi
08002234 g     F .text  00000020 rtc_alarm_isr
08002274  w    F .text  00000002 exti15_10_isr
080007b8 g     F .text  00000010 _ZN5codal23fiber_scheduler_runningEv
08000f70 g     F .text  000000fc _ZN5codal5Timer7triggerEv
0800197c g     F .text  00000004 _ZN5codal3_cm3Pin15getDigitalValueEv
08001de4 g     F .text  00000004 tcb_get_stack_base
08001718 g     F .text  00000014 _ZN5codal5EventC2Ev
080017a4 g     F .text  00000004 _ZN5codal13STM32BluePill12idleCallbackEv
08001dd0 g     F .text  00000006 tcb_allocate
080035fc g     O .text  00000018 _ZTVN5codal5TimerE
08002274  w    F .text  00000002 hard_fault_handler
08003138 g     F .text  00000094 _ZN5codal3I2C5writeEtPhib
08002274  w    F .text  00000002 exti1_isr
08001a04 g     F .text  00000006 _ZN5codal3_cm3Pin7setPullENS_8PullModeE
08003470 g     F .text  00000020 _ZN5codal14AbstractButtonD0Ev
08002274  w    F .text  00000002 i2c1_ev_isr
08001ca0 g     F .text  00000006 _ZN10DynamicPwmC1E7PinName
080033dc g     F .text  00000028 _ZN5codal6Button12buttonActiveEv
08002274  w    F .text  00000002 dma2_channel1_isr
08001108  w    F .text  0000004c _ZN5codal22MemberFunctionCallback10methodCallINS_10MessageBusEEEvPvPmNS_5EventE
080002b8  w    F .text  00000008 _ZN5codal10EventModel3addEPNS_8ListenerE
08001f0c g     F .text  00000018 enable_log
08002278  w    F .text  00000002 pend_sv_handler
080018dc g     F .text  00000008 __cxa_pure_virtual
08000508 g     F .text  000000e8 _ZN5codal15scheduler_eventENS_5EventE
08002874 g     F .text  00000030 rtc_check_flag
08002274  w    F .text  00000002 spi2_isr
08002a2c g     F .text  0000001c .hidden __floatsisf
08001bb8 g     F .text  00000048 _ZN5codal3_cm3Pin14setAnalogValueEi
08001d20 g     F .text  00000034 target_init
08000678 g     F .text  00000140 _ZN5codal14scheduler_initERNS_10EventModelE
080032b4  w    F .text  00000006 _ZN5codal3Pin7setPullENS_8PullModeE
08001940 g     F .text  00000004 _ZN5codal3_cm3I2C5writeEtPhib
080016f8 g     F .text  0000001e _ZN5codal5EventC1EttyNS_15EventLaunchModeE
08002274  w    F .text  00000002 tim8_up_isr
08003514 g     F .text  0000000c os_sem_init
08000278  w    F .text  00000040 _ZN5codal13STM32BluePillD0Ev
08003078 g     F .text  0000001c __cxa_atexit
08002c3c g     F .text  00000136 .hidden __aeabi_fdiv
08002274  w    F .text  00000002 dma2_channel2_isr
080028a4 g     F .text  00000014 flash_set_ws
080024c0 g     F .text  00000098 rcc_set_rtc_clock_source
080035e0  w    O .text  0000001c _ZTVN5codal14CodalComponentE
0800357a g     F .text  0000001e memcmp
08002a48 g     F .text  0000008c .hidden __floatundisf
080019dc g     F .text  0000000e _ZN5codal3_cm3Pin8isAnalogEv
08003074  w    F .text  00000002 .hidden __aeabi_ldiv0
08002278  w    F .text  00000002 debug_monitor_handler
08002274  w    F .text  00000002 exti3_isr
08002274  w    F .text  00000002 adc3_isr
080035b8 g     F .text  00000028 strncat
080001b8  w    F .text  00000020 _ZN5codal14CodalComponentD0Ev
080020c6 g       .text  00000000 save_context
08002274  w    F .text  00000002 tim3_isr
08002274  w    F .text  00000002 usart2_isr
08002a24 g     F .text  00000024 .hidden __aeabi_ui2f
08001da8  w    F .text  00000020 target_panic
08001fec g     F .text  0000000a debug_print
08002274  w    F .text  00000002 usb_lp_can_rx0_isr
08002044 g       .text  00000000 swap_context
08003860 g       .fini_array    00000000 __fini_array_start
08002274  w    F .text  00000002 i2c2_er_isr
08002624 g     F .text  0000008c rtc_awake_from_off
08003598 g     F .text  00000010 memset
2000021c g     O .bss   00000004 _ZN5codal12system_timerE
08000224 g     F .text  0000001c main
20000104 g     O .bss   000000f0 _ZN5codal14CodalComponent10componentsE
080032a0 g     F .text  0000000c _ZN5codal3I2CC1ERNS_3PinES2_
08002274  w    F .text  00000002 i2c2_ev_isr
080001fc g     F .text  00000026 _Z10Blink_mainRN5codal13STM32BluePillE
08002a58 g     F .text  0000007c .hidden __aeabi_l2f
08002274  w    F .text  00000002 uart5_isr
20000214 g     O .bss   00000008 heap
08002278  w    F .text  00000002 sys_tick_handler
08003614  w    O .text  00000014 _ZTVN5codal10EventModelE
08001360 g     F .text  00000084 _ZN5codal10MessageBusD1Ev
08003860 g       .init_array    00000000 __init_array_end
08002274  w    F .text  00000002 fsmc_isr
08002274  w    F .text  00000002 dma1_channel1_isr
08002274  w    F .text  00000002 exti4_isr
08001c70 g     F .text  00000030 _ZN5codal3_cm5TimerC2Ev
08003704 g     O .text  00000054 _ZTVN5codal3_cm3PinE
20000004 g     O .data  00000004 codal_heap_start
08000e1c g     F .text  00000038 _ZN5codal5TimerC2Ev
08001d5c g     F .text  00000024 target_wait_for_event
08000d28 g     F .text  0000002e _ZN5codal8ListenerC2EttPFvNS_5EventEEt
08001c00 g     F .text  00000024 _ZN5codal3_cm3Pin17setAnalogPeriodUsEi
08001f34 g     F .text  00000088 _Z11debug_printj
08002a48 g     F .text  0000008c .hidden __aeabi_ul2f
0800202c g     F .text  00000018 _Z13debug_printlnj
0800193c g     F .text  00000004 _ZN5codal3_cm3I2C12setFrequencyEm
08002ad4 g     F .text  00000168 .hidden __mulsf3
08001408 g     F .text  000000b0 _ZN5codal10MessageBusC1Ev
08001154 g     F .text  00000084 _ZN5codal10MessageBus6removeEPNS_8ListenerE
080023b4 g     F .text  0000001c exti_enable_request
08002364 g     F .text  00000050 exti_set_trigger
08002274  w    F .text  00000002 mem_manage_handler
080019b0 g     F .text  0000000e _ZN5codal3_cm3Pin7isInputEv
080019d0 g     F .text  0000000c _ZN5codal3_cm3Pin9isDigitalEv
20000010 g     O .data  00000004 rcc_apb2_frequency
080026b0 g     F .text  00000040 rtc_set_alarm_time
08001718 g     F .text  00000014 _ZN5codal5EventC1Ev
08003490 g     F .text  0000002c _ZN5codal14AbstractButtonC1Ev
080030b8 g     F .text  00000006 _ZN5codal3I2C4stopEv
08003854 g     F .fini  00000000 _fini
08001090 g     F .text  00000020 _ZN5codal28system_timer_current_time_usEv
08001d80 g     F .text  00000028 target_wait
08002274  w    F .text  00000002 can2_tx_isr
08001948 g     F .text  0000001c _ZN5codal3_cm3I2CC1ERNS_3PinES3_
08000ad4 g     F .text  000000a0 _ZN5codal6invokeEPFvPvES0_
20000290 g     O .bss   00000404 codalLogStore
08002274  w    F .text  00000002 exti9_5_isr
0800191c g     F .text  00000010 led_off
080032c4 g     F .text  00000014 _ZN5codal6ButtonD1Ev
08001964  w    F .text  00000006 _ZN5codal3Pin15setServoPulseUsEm
08002274  w    F .text  00000002 dma2_channel3_isr
080028b8 g     F .text  0000016c .hidden __aeabi_frsub
080024b0 g     F .text  00000010 rcc_enable_rtc_clock
080012c0 g     F .text  000000a0 _Z14async_callbackPv
08003830 g     O .text  00000020 _ZTVN5codal14AbstractButtonE
08002ad4 g     F .text  00000168 .hidden __aeabi_fmul
0800019c  w    F .text  00000002 _ZN5codal14CodalComponent12idleCallbackEv
08003858 g       .preinit_array 00000000 __preinit_array_end
08002258 g     F .text  0000001a nvic_enable_irq
0800192c g     F .text  00000010 led_wait
08001d1c g     F .text  00000004 target_enable_debug
08001844 g     F .text  0000003c _ZN5codal15STM32BluePillIOC1Ev
08002274  w    F .text  00000002 dma1_channel7_isr
08002168 g     F .text  0000008c platform_start_timer
08000d28 g     F .text  0000002e _ZN5codal8ListenerC1EttPFvNS_5EventEEt
20000000 g       .data  00000000 _data
08002274  w    F .text  00000002 tim1_up_isr
080026f0 g     F .text  00000040 rtc_set_prescale_val
0800380c g     O .text  00000024 _ZTVN5codal6ButtonE
08001b60 g     F .text  0000001c _ZN5codal3_cm3PinC2EihNS_13PinCapabilityE
08001c44 g     F .text  0000002c _ZN5codal3_cm5Timer11syncRequestEv
08001b28 g     F .text  00000038 _ZN5codal3_cm3Pin15getDigitalValueENS_8PullModeE
20000224 g     O .bss   00000004 _ZN5codal3_cm5Timer8instanceE
08001948 g     F .text  0000001c _ZN5codal3_cm3I2CC2ERNS_3PinES3_
08002558 g     F .text  000000cc rcc_clock_setup_in_hse_8mhz_out_72mhz
080032bc g     F .text  00000008 _ZN5codal6Button9isPressedEv
08001c34 g     F .text  00000010 _ZN5codal3_cm13tick_callbackEv
08001784 g     F .text  00000020 _ZN5codal22MemberFunctionCallbackeqERKS0_
08002730 g     F .text  00000018 rtc_get_counter_val
0800196c  w    F .text  00000006 _ZN5codal3Pin17setAnalogPeriodUsEm
08001c24 g     F .text  0000000c _ZN5codal3_cm3Pin15setAnalogPeriodEi
08002274  w    F .text  00000002 can2_sce_isr
08002274  w    F .text  00000002 tim4_isr
08001840 g     F .text  00000004 _ZN5codal13STM32BluePill11enableDebugEv
080037e0 g     O .text  0000002c _ZTVN5codal3I2CE
080014b8 g     F .text  00000024 _ZN5codal10MessageBus12dequeueEventEv
080014dc g     F .text  00000054 _ZN5codal10MessageBus21deleteMarkedListenersEv
20000014 g       .data  00000000 _edata
08000b74 g     F .text  00000028 _ZN5codal22launch_new_fiber_paramEPFvPvES2_S0_
080011d8 g     F .text  000000e8 _ZN5codal10MessageBus3addEPNS_8ListenerE
08002274  w    F .text  00000002 dma1_channel2_isr
08002274  w    F .text  00000002 i2c1_er_isr
080032f4 g     F .text  000000e6 _ZN5codal6Button16periodicCallbackEv
080005f0 g     F .text  00000054 _Z15getFiberContextv
08001d54 g     F .text  00000004 target_enable_irq
08000e04 g     F .text  0000000a _ZN5codal5Timer17disableInterruptsEv
08002274  w    F .text  00000002 can_sce_isr
08002278  w    F .text  00000002 nmi_handler
080027e4 g     F .text  0000005c rtc_interrupt_disable
080018e4 g     F .text  00000004 _Znwj
08003628 g     O .text  0000003c _ZTVN5codal10MessageBusE
08000b9c g     F .text  00000018 _ZN5codal9idle_taskEv
08001cac g     F .text  00000004 _ZN10DynamicPwm10getPinNameEv
08002a24 g     F .text  00000024 .hidden __floatunsisf
08001400 g     F .text  00000008 _ZThn8_N5codal10MessageBusD0Ev
08002274  w    F .text  00000002 tim8_cc_isr
080017f4 g     F .text  0000004c _ZN5codal13STM32BluePillC1Ev
080010f0 g     F .text  00000016 _ZN5codal10MessageBus9elementAtEi
080013e4 g     F .text  00000008 _ZThn8_N5codal10MessageBusD1Ev
08001c30 g     F .text  00000002 _ZN5codal3_cm5Timer9triggerInEy
080009f8 g     F .text  00000014 _ZN5codal8scheduleEv
08001cf4 g     F .text  0000001c _Z25stm32bluepill_dmesg_flushv
08001980 g     F .text  0000002c _ZN5codal3_cm3Pin13setServoValueEiii
08002274  w    F .text  00000002 tamper_isr
08001ca0 g     F .text  00000006 _ZN10DynamicPwmC2E7PinName
08003074  w    F .text  00000002 .hidden __aeabi_idiv0
08003858 g       .init_array    00000000 __init_array_start
080023e0 g     F .text  00000006 gpio_clear
08000338 g     F .text  000000dc _ZN5codal14CodalComponent12addComponentEv
08002274  w    F .text  00000002 eth_wkup_isr
080018cc g     F .text  00000010 enable_debug
08001658 g     F .text  00000046 _ZN5codal10MessageBus4idleENS_5EventE
080016d8 g     F .text  0000001e _ZN5codal5EventC2EttNS_15EventLaunchModeE
080010e0  w    F .text  00000010 _ZN5codal10EventModel4sendENS_5EventE
08003520 g     F .text  00000044 os_task_init
080015d0 g     F .text  0000006e _ZN5codal10MessageBus10queueEventERNS_5EventE
080035a8 g     F .text  00000010 strlen
08001dd8 g     F .text  00000004 tcb_configure_lr
20005000 g       .bss   00000000 _stack
080023dc g     F .text  00000004 gpio_set
08000d6c g     F .text  00000096 _ZN5codal8Listener5queueENS_5EventE
08002274  w    F .text  00000002 bus_fault_handler
08002274  w    F .text  00000002 wwdg_isr
08002274  w    F .text  00000002 dma1_channel3_isr
08000e10 g     F .text  0000000a _ZN5codal5Timer16enableInterruptsEv
08002748 g     F .text  00000040 rtc_set_counter_val
0800106c g     F .text  00000002 _ZN5codal5TimerD1Ev
080034f0 g     F .text  00000024 os_msgQ_init
08003858 g       .preinit_array 00000000 __preinit_array_start
08002274  w    F .text  00000002 spi3_isr
08003570 g     F .text  0000000a __aeabi_atexit
08002254 g     F .text  00000004 millis
080028c4 g     F .text  00000160 .hidden __addsf3
08001a0c g     F .text  00000054 _ZN5codal3_cm3Pin9isTouchedEv
08001d10 g     F .text  0000000c target_set_tick_callback
08002274  w    F .text  00000002 tim2_isr
08000ccc g     F .text  00000040 free
08002274  w    F .text  00000002 exti0_isr
080030b8 g     F .text  00000006 _ZN5codal3I2C5startEv

C:\tmp\codal>
```
