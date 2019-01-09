# Build codal-libopencm3, the CODAL framework ported to libopencm3 for STM32 Blue Pill.
# Also builds the libepencm3 and newlib libraries.  cmake version in Yotta Docker image in 3.8.2.
project(codal-libopencm3)

# enable verbose log. Must be after project()
# set(CMAKE_VERBOSE_MAKEFILE ON)
# set(ENV{VERBOSE} "1")
# message("VERBOSE: $ENV{VERBOSE}")

# apply codal build settings
include("${CODAL_UTILS_LOCATION}")

# set the build for min size, i.e. -Os -DNDEBUG
# set(CMAKE_BUILD_TYPE MinSizeRel PARENT_SCOPE)

# CMAKE_CURRENT_SOURCE_DIR is codal/libraries/codal-libopencm3
# build the libraries in the "lib" folder.
set(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib")
# files to be patched.
set(PATCHES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/patches")

###############################################################################
# codal-core

set(CODAL_CORE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../codal-core")
message("CODAL_CORE_DIR: ${CODAL_CORE_DIR}")

# find codal-core sources and headers.
# RECURSIVE_FIND_DIR(CODAL_CORE_INC "${CODAL_CORE_DIR}/inc" "*.h")
set(CODAL_CORE_INC
    "${CODAL_CORE_DIR}/inc"
    "${CODAL_CORE_DIR}/inc/core"
    "${CODAL_CORE_DIR}/inc/driver-models"
    "${CODAL_CORE_DIR}/inc/drivers"
    "${CODAL_CORE_DIR}/inc/streams"
    "${CODAL_CORE_DIR}/inc/types"
)
RECURSIVE_FIND_FILE(CODAL_CORE_SRC "${CODAL_CORE_DIR}/source" "*.cpp")

# skip codal-core library, already created by parent.
# add_library(codal-core ${CODAL_CORE_SRC})

###############################################################################
# nano-float

set(NANOFLOAT_DIR "${LIB_DIR}/nano-float")
message("NANOFLOAT_DIR: ${NANOFLOAT_DIR}")

# find sources and headers.
set(NANOFLOAT_INC "${NANOFLOAT_DIR}/inc")
RECURSIVE_FIND_FILE(NANOFLOAT_SRC "${NANOFLOAT_DIR}/src" "*.c")

# create library.
add_library(nano-float ${NANOFLOAT_SRC})

###############################################################################
# cocoos

set(COCOOS_DIR "${LIB_DIR}/cocoOS_5.0.3")
message("COCOOS_DIR: ${COCOOS_DIR}")

# find cocoos sources and headers.
set(COCOOS_INC "${COCOOS_DIR}/inc")
RECURSIVE_FIND_FILE(COCOOS_SRC "${COCOOS_DIR}/src" "*.c")

# create cocoos library.
add_library(cocoos ${COCOOS_SRC})

###############################################################################
# bme280

set(BME280_DIR "${LIB_DIR}/BME280")
message("BME280_DIR: ${BME280_DIR}")

# find bme280 sources and headers.
set(BME280_INC "${BME280_DIR}/src")
RECURSIVE_FIND_FILE(BME280_SRC "${BME280_DIR}/src" "*.cpp")

# create bme280 library.
add_library(bme280 ${BME280_SRC})

###############################################################################
# boost

set(BOOST_DIR "${LIB_DIR}/boost_")
message("BOOST_DIR: ${BOOST_DIR}")

# find boost sources and headers.
set(BOOST_INC
    "${LIB_DIR}/boost_type_traits/src" 
    "${LIB_DIR}/boost_assert/src" 
    "${LIB_DIR}/boost_config/src" 
    "${LIB_DIR}/boost_core/src" 
    "${LIB_DIR}/boost_utility/src" 
    "${LIB_DIR}/boost_predef/src" 
    "${LIB_DIR}/boost_mpl/src" 
    "${LIB_DIR}/boost_detail/src" 
    "${LIB_DIR}/boost_parameter/src" 
    "${LIB_DIR}/boost_iterator/src" 
    "${LIB_DIR}/boost_lockfree/src" 
    "${LIB_DIR}/boost_static_assert/src" 
    "${LIB_DIR}/boost_preprocessor/src"
)
RECURSIVE_FIND_FILE(BOOST_SRC "${BOOST_DIR}*/src" "*.cpp")

# no need to create boost library since all files are headers.
# add_library(boost ${BOOST_SRC})

###############################################################################
# stm32

set(STM32_DIR "${CMAKE_CURRENT_SOURCE_DIR}/stm32")
message("STM32_DIR: ${STM32_DIR}")

set(STM32_INC "${STM32_DIR}")

###############################################################################
# stm32/logger

set(LOGGER_DIR "${STM32_DIR}/logger")
message("LOGGER_DIR: ${LOGGER_DIR}")

# find logger sources and headers.
RECURSIVE_FIND_DIR(LOGGER_INC  "${LOGGER_DIR}" "*.h")
RECURSIVE_FIND_FILE(LOGGER_SRC "${LOGGER_DIR}" "*.cpp")

# create logger library.
add_library(logger ${LOGGER_SRC})

###############################################################################
# stm32/bluepill

set(BLUEPILL_DIR "${STM32_DIR}/bluepill")
message("BLUEPILL_DIR: ${BLUEPILL_DIR}")

# find bluepill sources and headers.
RECURSIVE_FIND_DIR(BLUEPILL_INC  "${BLUEPILL_DIR}" "*.h")
RECURSIVE_FIND_FILE(BLUEPILL_SRC "${BLUEPILL_DIR}" "*.cpp")

# create bluepill library.
add_library(bluepill ${BLUEPILL_SRC})

###############################################################################
# stm32/baseloader

set(BASELOADER_DIR "${STM32_DIR}/baseloader")
message("BASELOADER_DIR: ${BASELOADER_DIR}")

# find baseloader sources and headers.
set(BASELOADER_INC
    "${BASELOADER_DIR}" 
)
SOURCE_FILES(BASELOADER_SRC "${BASELOADER_DIR}" "*.c")

# create baseloader library.
add_library(baseloader ${BASELOADER_SRC})

###############################################################################
# stm32/bootloader

set(BOOTLOADER_DIR "${STM32_DIR}/bootloader")
message("BOOTLOADER_DIR: ${BOOTLOADER_DIR}")

# find bootloader sources and headers.
set(BOOTLOADER_INC
    "${BOOTLOADER_DIR}/stm32f103" 
    "${BOOTLOADER_DIR}/stm32f103/generic" 
)
SOURCE_FILES(BOOTLOADER_SRC "${BOOTLOADER_DIR}" "*.c")
SOURCE_FILES(BOOTLOADER_STM32F103 "${BOOTLOADER_DIR}/stm32f103" "*.c")
LIST(APPEND BOOTLOADER_SRC ${BOOTLOADER_STM32F103})

# create bootloader library.
add_library(bootloader ${BOOTLOADER_SRC})

###############################################################################
# stm32/hal

set(HAL_DIR "${STM32_DIR}/hal")
message("HAL_DIR: ${HAL_DIR}")

# find hal sources and headers.
set(HAL_INC "${HAL_DIR}")
RECURSIVE_FIND_FILE(HAL_SRC "${HAL_DIR}"   "*.cpp")
RECURSIVE_FIND_FILE(HAL_C   "${HAL_DIR}"   "*.c")
LIST(
    APPEND 
    HAL_SRC
    ${HAL_C}
    "${CMAKE_CURRENT_SOURCE_DIR}/asm/CortexContextSwitch.s"
)

add_library(hal ${HAL_SRC})

# ENABLE_EXPORTS must be enabled to allow .o files to be linked with an executable
set(ENABLE_EXPORTS ON)

# create hal object files (.o) instead of library (.a). TODO: add_library(... $<TARGET_OBJECTS:hal> ...)
add_library(hal_obj OBJECT ${HAL_SRC})

#"${HAL_DIR}/reset_handler.c"
#"${HAL_DIR}/codal_target_hal.cpp"
#"${HAL_DIR}/unit_test.c"

###############################################################################
# stm32/model

set(MODEL_DIR "${STM32_DIR}/model")
message("MODEL_DIR: ${MODEL_DIR}")

# find model sources and headers.
RECURSIVE_FIND_DIR(MODEL_INC  "${MODEL_DIR}" "*.h")
RECURSIVE_FIND_FILE(MODEL_SRC "${MODEL_DIR}" "*.cpp")

# create model library.
add_library(model ${MODEL_SRC})

###############################################################################
# stm32/cm

set(CM_DIR "${STM32_DIR}/cm")
message("CM_DIR: ${CM_DIR}")

# find cm sources and headers.
RECURSIVE_FIND_DIR(CM_INC  "${CM_DIR}" "*.h")
RECURSIVE_FIND_FILE(CM_SRC "${CM_DIR}" "*.cpp")

# create cm library.
add_library(cm ${CM_SRC})

###############################################################################
# stm32/adcint

set(ADCINT_DIR "${STM32_DIR}/adcint")
message("ADCINT_DIR: ${ADCINT_DIR}")

# find adcint sources and headers.
RECURSIVE_FIND_DIR(ADCINT_INC  "${ADCINT_DIR}" "*.h")
RECURSIVE_FIND_FILE(ADCINT_SRC "${ADCINT_DIR}" "*.cpp")

# create adcint library.
add_library(adcint ${ADCINT_SRC})

###############################################################################
# stm32/i2cint

set(I2CINT_DIR "${STM32_DIR}/i2cint")
message("I2CINT_DIR: ${I2CINT_DIR}")

# find i2cint sources and headers.
RECURSIVE_FIND_DIR(I2CINT_INC  "${I2CINT_DIR}" "*.h")
RECURSIVE_FIND_FILE(I2CINT_SRC "${I2CINT_DIR}" "*.cpp")

# create i2cint library.
add_library(i2cint ${I2CINT_SRC})

###############################################################################
# stm32/porting

set(PORTING_DIR "${STM32_DIR}/porting")
message("PORTING_DIR: ${PORTING_DIR}")

# find porting sources and headers.
RECURSIVE_FIND_DIR(PORTING_INC  "${PORTING_DIR}" "*.h")
RECURSIVE_FIND_FILE(PORTING_SRC "${PORTING_DIR}" "*.cpp")

# create porting library.
add_library(porting ${PORTING_SRC})

###############################################################################
# stm32/simulator

set(SIMULATOR_DIR "${STM32_DIR}/simulator")
message("SIMULATOR_DIR: ${SIMULATOR_DIR}")

# find simulator sources and headers.
RECURSIVE_FIND_DIR(SIMULATOR_INC  "${SIMULATOR_DIR}" "*.h")
RECURSIVE_FIND_FILE(SIMULATOR_SRC "${SIMULATOR_DIR}" "*.cpp")

# create simulator library.
add_library(simulator ${SIMULATOR_SRC})

###############################################################################
# stm32/spiint

set(SPIINT_DIR "${STM32_DIR}/spiint")
message("SPIINT_DIR: ${SPIINT_DIR}")

# find spiint sources and headers.
RECURSIVE_FIND_DIR(SPIINT_INC  "${SPIINT_DIR}" "*.h")
RECURSIVE_FIND_FILE(SPIINT_SRC "${SPIINT_DIR}" "*.cpp")

# create spiint library.
add_library(spiint ${SPIINT_SRC})

###############################################################################
# stm32/uartint

set(UARTINT_DIR "${STM32_DIR}/uartint")
message("UARTINT_DIR: ${UARTINT_DIR}")

# find uartint sources and headers.
RECURSIVE_FIND_DIR(UARTINT_INC  "${UARTINT_DIR}" "*.h")
RECURSIVE_FIND_FILE(UARTINT_SRC "${UARTINT_DIR}" "*.cpp")

# create uartint library.
add_library(uartint ${UARTINT_SRC})

###############################################################################
# newlib

set(NEWLIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../newlib")
message("NEWLIB_DIR: ${NEWLIB_DIR}")

# find newlib headers.
set(
    NEWLIB_INC
    "${NEWLIB_DIR}/include"
)

###############################################################################
# qfplib

set(QFPLIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../qfplib")
message("QFPLIB_DIR: ${QFPLIB_DIR}")

# find headers.
set(
    QFPLIB_INC
    "${QFPLIB_DIR}"
)

###############################################################################
# libopencm3

set(LIBOPENCM3_DIR "${LIB_DIR}/libopencm3")
message("LIBOPENCM3_DIR: ${LIBOPENCM3_DIR}")

# patch nvic.h, vector_nvic.c, which are auto-generated.  TODO: Allow other platforms besides STM32F1
set(LIBOPENCM3_PATCH "${PATCHES_DIR}/libopencm3")
set(PATCH_NVIC "include/libopencm3/stm32/f1/nvic.h")
set(PATCH_VECTOR "lib/stm32/f1/vector_nvic.c")
execute_process(WORKING_DIRECTORY "${LIB_DIR}" 
    COMMAND "cp" "${LIBOPENCM3_PATCH}/${PATCH_NVIC}" "${LIBOPENCM3_DIR}/${PATCH_NVIC}")
execute_process(WORKING_DIRECTORY "${LIB_DIR}" 
    COMMAND "cp" "${LIBOPENCM3_PATCH}/${PATCH_VECTOR}" "${LIBOPENCM3_DIR}/${PATCH_VECTOR}")

# remove files we shouldn't compile.  TODO: Allow other platforms besides STM32F1
execute_process(WORKING_DIRECTORY "${LIBOPENCM3_DIR}" COMMAND "rm" "lib/stm32/common/adc_common_v2.c")
execute_process(WORKING_DIRECTORY "${LIBOPENCM3_DIR}" COMMAND "rm" "lib/stm32/common/adc_common_v2_multi.c")
execute_process(WORKING_DIRECTORY "${LIBOPENCM3_DIR}" COMMAND "rm" "lib/stm32/common/crc_v2.c")
execute_process(WORKING_DIRECTORY "${LIBOPENCM3_DIR}" COMMAND "rm" "lib/stm32/common/crs_common_all.c")
execute_process(WORKING_DIRECTORY "${LIBOPENCM3_DIR}" COMMAND "rm" "lib/stm32/common/crypto_common_f24.c")
execute_process(WORKING_DIRECTORY "${LIBOPENCM3_DIR}" COMMAND "rm" "lib/stm32/common/flash_common_f24.c")
execute_process(WORKING_DIRECTORY "${LIBOPENCM3_DIR}" COMMAND "rm" "lib/stm32/common/dma_common_f24.c")
execute_process(WORKING_DIRECTORY "${LIBOPENCM3_DIR}" COMMAND "rm" "lib/stm32/common/hash_common_f24.c")
execute_process(WORKING_DIRECTORY "${LIBOPENCM3_DIR}" COMMAND "rm" "lib/stm32/common/timer_common_f24.c")
execute_process(WORKING_DIRECTORY "${LIBOPENCM3_DIR}" COMMAND "rm" "lib/stm32/common/gpio_common_f0234.c")
execute_process(WORKING_DIRECTORY "${LIBOPENCM3_DIR}" COMMAND "rm" "lib/stm32/common/rtc_common_l1f024.c")
execute_process(WORKING_DIRECTORY "${LIBOPENCM3_DIR}" COMMAND "rm" "lib/stm32/common/timer_common_f0234.c")
execute_process(WORKING_DIRECTORY "${LIBOPENCM3_DIR}" COMMAND "rm" "lib/stm32/common/i2c_common_v2.c")
execute_process(WORKING_DIRECTORY "${LIBOPENCM3_DIR}" COMMAND "rm" "lib/stm32/common/pwr_common_v2.c")
execute_process(WORKING_DIRECTORY "${LIBOPENCM3_DIR}" COMMAND "rm" "lib/stm32/common/spi_common_v2.c")
execute_process(WORKING_DIRECTORY "${LIBOPENCM3_DIR}" COMMAND "rm" "lib/stm32/common/usart_common_v2.c")
execute_process(WORKING_DIRECTORY "${LIBOPENCM3_DIR}" COMMAND "rm" "lib/stm32/common/flash_common_l01.c")
execute_process(WORKING_DIRECTORY "${LIBOPENCM3_DIR}" COMMAND "rm" "lib/stm32/common/spi_common_v1_frf.c")
execute_process(WORKING_DIRECTORY "${LIBOPENCM3_DIR}" COMMAND "rm" "lib/stm32/common/rng_common_v1.c")

# find libopencm3 sources and headers.   TODO: Allow other platforms besides STM32F1
set(LIBOPENCM3_INC "${LIBOPENCM3_DIR}/include")
set(
    LIBOPENCM3_CM3
    "${LIBOPENCM3_DIR}/lib/cm3/nvic.c"
    "${LIBOPENCM3_DIR}/lib/cm3/vector.c"
    "${LIBOPENCM3_DIR}/lib/cm3/scb.c"
)
RECURSIVE_FIND_FILE(LIBOPENCM3_COMMON "${LIBOPENCM3_DIR}/lib/stm32/common" "*.c")
set(  # source files for STM32F1*
    LIBOPENCM3_F1
    "${LIBOPENCM3_DIR}/lib/stm32/desig.c"
    "${LIBOPENCM3_DIR}/lib/stm32/f1/adc.c"
    "${LIBOPENCM3_DIR}/lib/stm32/f1/dac.c"
    "${LIBOPENCM3_DIR}/lib/stm32/f1/dma.c"
    "${LIBOPENCM3_DIR}/lib/stm32/f1/flash.c"
    "${LIBOPENCM3_DIR}/lib/stm32/f1/gpio.c"
    "${LIBOPENCM3_DIR}/lib/stm32/f1/i2c.c"
    "${LIBOPENCM3_DIR}/lib/stm32/f1/pwr.c"
    "${LIBOPENCM3_DIR}/lib/stm32/f1/rcc.c"
    "${LIBOPENCM3_DIR}/lib/stm32/f1/rtc.c"
    "${LIBOPENCM3_DIR}/lib/stm32/f1/timer.c"
    # vector_nvic.c should not be compiled directly, causes error "aliased to undefined symbol 'blocking_handler'"
    # "${LIBOPENCM3_DIR}"/lib/stm32/f1/vector_nvic.c
)
set(  # source files for usb
    LIBOPENCM3_USB
    "${LIBOPENCM3_DIR}/lib/stm32/common/st_usbfs_core.c"
    "${LIBOPENCM3_DIR}/lib/stm32/st_usbfs_v1.c"
    "${LIBOPENCM3_DIR}/lib/usb/usb.c"
    "${LIBOPENCM3_DIR}/lib/usb/usb_control.c"
    "${LIBOPENCM3_DIR}/lib/usb/usb_standard.c"
)

# create libopencm3 library.
add_library(libopencm3 
    ${LIBOPENCM3_CM3}
    ${LIBOPENCM3_COMMON}
    ${LIBOPENCM3_F1}
    ${LIBOPENCM3_USB}
)

###############################################################################
# codal-libopencm3

set(CODAL_LIBOPENCM3_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
message("CODAL_LIBOPENCM3_DIR: ${CODAL_LIBOPENCM3_DIR}")

# find codal-libopencm3 sources and headers. newlib must be first
set(
    TOP_LEVEL_INCLUDE_DIRS
    "${NEWLIB_INC}"
    "${CODAL_LIBOPENCM3_DIR}"
)
message("TOP_LEVEL_INCLUDE_DIRS: ${TOP_LEVEL_INCLUDE_DIRS}")
set(
    TOP_LEVEL_SOURCE_FILES
    "${CODAL_LIBOPENCM3_DIR}/sensor_display.cpp"
)

# create our codal-libopencm3 target.
add_library(codal-libopencm3 ${TOP_LEVEL_SOURCE_FILES})
message("TOP_LEVEL_SOURCE_FILES: ${TOP_LEVEL_SOURCE_FILES}")

###############################################################################
# global settings

# CMAKE_CURRENT_BINARY_DIR = ~/codal/build/libraries/codal-libopencm3
message("CMAKE_CURRENT_BINARY_DIR: ${CMAKE_CURRENT_BINARY_DIR}")
string(TOLOWER "${CODAL_TARGET_PROCESSOR}" TARGET_PROCESSOR)

# set linker script.  Must specify reset_handler to avoid linking in reset_handler from libopencm3/vector.c
set(LINKER_SCRIPT " -T\"${CMAKE_CURRENT_LIST_DIR}/ld/stm32f103x8.ld\" ")
set(LINKER_OBJ    " -Wl,\"${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/hal.dir/stm32/hal/reset_handler.c.o\" ")

set(CMAKE_LINKER_FLAGS     "${CMAKE_LINKER_FLAGS}     ${LINKER_OBJ} ${LINKER_SCRIPT}" PARENT_SCOPE)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${LINKER_OBJ} ${LINKER_SCRIPT}" PARENT_SCOPE)
set(CMAKE_SYSTEM_PROCESSOR "armv7-m" PARENT_SCOPE)

# "stm32/bootloader" requires "-std=gnu11" flag or it will fail for Unicode strings like u"..."
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu11")

# include directories, skip bootloader because ghostfat.h conflicts with Codal.
include_directories(    
    ${INCLUDE_DIRS} 
    ${TOP_LEVEL_INCLUDE_DIRS}
    ${CODAL_CORE_INC}
    ${LIBOPENCM3_INC}
    ${COCOOS_INC}
    ${QFPLIB_INC}
    ${NANOFLOAT_INC}
    ${STM32_INC}
    ${LOGGER_INC}
    ${BLUEPILL_INC}
    ${BASELOADER_INC}
    ${BOOTLOADER_INC}
    ${HAL_INC}
    ${ADCINT_INC}
    ${I2CINT_INC}
    ${PORTING_INC}
    ${SIMULATOR_INC}
    ${SPIINT_INC}
    ${UARTINT_INC}
    ${CM_INC}
    ${MODEL_INC}
    ${BME280_INC}
    ${BOOST_INC}
    ${LIB_DIR}
)

# libraries to be built
target_link_libraries(
    codal-libopencm3
    codal-core
    libopencm3
    cocoos
    qfplib
    nano-float
    newlib
    logger
    bluepill
    baseloader
    bootloader
    hal
    # hal_obj
    adcint
    i2cint
    porting
    simulator
    spiint 
    uartint
    cm
    model
    bme280
)

# MODEL_INCLUDE_DIRS will be exposed to the main application.
set(
    MODEL_INCLUDE_DIRS 
    "${LOGGER_DIR}"
    "${BLUEPILL_DIR}"
    "${BASELOADER_DIR}"
    "${HAL_DIR}"
    "${ADCINT_DIR}"
    "${I2CINT_DIR}"
    "${PORTING_DIR}"
    "${SIMULATOR_DIR}"
    "${SPIINT_DIR}"
    "${UARTINT_DIR}"
    "${CM_DIR}"
    "${MODEL_DIR}"
)

# expose it to parent cmake.
target_include_directories(
    codal-libopencm3 
    PUBLIC 
    ${MODEL_INCLUDE_DIRS} 
    ${TOP_LEVEL_INCLUDE_DIRS}
)
