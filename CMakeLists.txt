# Build codal-libopencm3, the CODAL framework ported to libopencm3 for STM32 Blue Pill.
# Also builds the libepencm3 and newlib libraries.

# enable verbose log
set(CMAKE_VERBOSE_MAKEFILE ON)
set(ENV{VERBOSE} "1")
message("VERBOSE: $ENV{VERBOSE}")

project(codal-libopencm3)
include("${CODAL_UTILS_LOCATION}")

# configure version of libopencm3 to clone.
set(LIBOPENCM3_VERSION "53347c266bb86f99b74c5f028c4c0f0c24d66b83") # Nov 6, 2018

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
set(STM32_BLUEPILL_DIR "${STM32_DIR}/bluepill")
set(STM32_CM_DIR "${STM32_DIR}/cm")
message("STM32_DIR: ${STM32_DIR}")

# find stm32 sources and headers.
RECURSIVE_FIND_DIR(STM32_INC "${STM32_DIR}" "*.h")
RECURSIVE_FIND_FILE(STM32_SRC "${STM32_DIR}" "*.cpp")
LIST(APPEND STM32_SRC "${CMAKE_CURRENT_SOURCE_DIR}/asm/CortexContextSwitch.s")
# MODEL_INCLUDE_DIRS will be exposed to the main application.
set(
    MODEL_INCLUDE_DIRS 
    "${STM32_BLUEPILL_DIR}"
    "${STM32_CM_DIR}"
)

# create stm32 library.
add_library(stm32 ${STM32_SRC})

###############################################################################
# newlib

set(NEWLIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../newlib")
message("NEWLIB_DIR: ${NEWLIB_DIR}")

# find newlib headers.
set(
    NEWLIB_INC
    "${NEWLIB_DIR}/include"
)

# set(NEWLIB_DIR             "${LIB_DIR}/newlib")
# set(NEWLIB_CTYPE_DIR       "${NEWLIB_DIR}/newlib/libc/ctype")
# set(NEWLIB_STRING_DIR      "${NEWLIB_DIR}/newlib/libc/string")
# set(NEWLIB_LIBM_COMMON_DIR "${NEWLIB_DIR}/newlib/libm/common")
# set(NEWLIB_LIBM_MATH_DIR   "${NEWLIB_DIR}/newlib/libm/math")
# message("NEWLIB_DIR: ${NEWLIB_DIR}")

# # clone libopencm3 into codal/libraries/codal-libopencm3/newlib
# # TODO: "--branch" "${NEWLIB_VERSION}" "--single-branch"
# execute_process(
#     WORKING_DIRECTORY "${LIB_DIR}" 
#     COMMAND "git" "clone" "https://github.com/lupyuen/newlib" 
#     OUTPUT_VARIABLE git_clone_newlib)

# # find newlib sources and headers.
# set(
#     NEWLIB_INC
#     "${NEWLIB_DIR}/include"
#     #"${NEWLIB_DIR}/newlib/libc/include"
#     #"../../libc/include"
#     "${NEWLIB_CTYPE_DIR}"
#     "${NEWLIB_LIBM_COMMON_DIR}"
# )
# # RECURSIVE_FIND_FILE(NEWLIB_CTYPE       "${NEWLIB_CTYPE_DIR}"       "*.c")
# # RECURSIVE_FIND_FILE(NEWLIB_STRING      "${NEWLIB_STRING_DIR}"      "*.c")
# RECURSIVE_FIND_FILE(NEWLIB_LIBM_COMMON "${NEWLIB_LIBM_COMMON_DIR}" "*.c")
# RECURSIVE_FIND_FILE(NEWLIB_LIBM_MATH   "${NEWLIB_LIBM_MATH_DIR}"   "*.c")

# set(
#     NEWLIB_CTYPE
#     "${NEWLIB_CTYPE_DIR}/ctype_.c"
#     "${NEWLIB_CTYPE_DIR}/isalnum.c"
#     "${NEWLIB_CTYPE_DIR}/isalpha.c"
#     "${NEWLIB_CTYPE_DIR}/iscntrl.c"
#     "${NEWLIB_CTYPE_DIR}/isdigit.c"
#     "${NEWLIB_CTYPE_DIR}/islower.c"
#     "${NEWLIB_CTYPE_DIR}/isupper.c"
#     "${NEWLIB_CTYPE_DIR}/isprint.c"
#     "${NEWLIB_CTYPE_DIR}/ispunct.c"
#     "${NEWLIB_CTYPE_DIR}/isspace.c"
#     "${NEWLIB_CTYPE_DIR}/isxdigit.c"
#     "${NEWLIB_CTYPE_DIR}/tolower.c"
#     "${NEWLIB_CTYPE_DIR}/toupper.c"
# )

# set(
#     NEWLIB_STRING
#     "${NEWLIB_STRING_DIR}/bcopy.c"
#     "${NEWLIB_STRING_DIR}/bzero.c"
#     "${NEWLIB_STRING_DIR}/explicit_bzero.c"
#     "${NEWLIB_STRING_DIR}/ffsl.c"
#     "${NEWLIB_STRING_DIR}/ffsll.c"
#     "${NEWLIB_STRING_DIR}/fls.c"
#     "${NEWLIB_STRING_DIR}/flsl.c"
#     "${NEWLIB_STRING_DIR}/flsll.c"
#     "${NEWLIB_STRING_DIR}/index.c"
#     "${NEWLIB_STRING_DIR}/memchr.c"
#     "${NEWLIB_STRING_DIR}/memcmp.c"
#     "${NEWLIB_STRING_DIR}/memcpy.c"
#     "${NEWLIB_STRING_DIR}/memmove.c"
#     "${NEWLIB_STRING_DIR}/memset.c"
#     "${NEWLIB_STRING_DIR}/rindex.c"
#     "${NEWLIB_STRING_DIR}/strcasecmp.c"
#     "${NEWLIB_STRING_DIR}/strcat.c"
#     "${NEWLIB_STRING_DIR}/strchr.c"
#     "${NEWLIB_STRING_DIR}/strcmp.c"
#     "${NEWLIB_STRING_DIR}/strcoll.c"
#     "${NEWLIB_STRING_DIR}/strcpy.c"
#     "${NEWLIB_STRING_DIR}/strcspn.c"
#     "${NEWLIB_STRING_DIR}/strdup.c"
#     "${NEWLIB_STRING_DIR}/strdup_r.c"
#     #"${NEWLIB_STRING_DIR}/strerror.c"
#     "${NEWLIB_STRING_DIR}/strerror_r.c"
#     "${NEWLIB_STRING_DIR}/strlcat.c"
#     "${NEWLIB_STRING_DIR}/strlcpy.c"
#     "${NEWLIB_STRING_DIR}/strlen.c"
#     "${NEWLIB_STRING_DIR}/strlwr.c"
#     "${NEWLIB_STRING_DIR}/strncasecmp.c"
#     "${NEWLIB_STRING_DIR}/strncat.c"
#     "${NEWLIB_STRING_DIR}/strncmp.c"
#     "${NEWLIB_STRING_DIR}/strncpy.c"
#     "${NEWLIB_STRING_DIR}/strnlen.c"
#     "${NEWLIB_STRING_DIR}/strnstr.c"
#     "${NEWLIB_STRING_DIR}/strpbrk.c"
#     "${NEWLIB_STRING_DIR}/strrchr.c"
#     "${NEWLIB_STRING_DIR}/strsep.c"
#     "${NEWLIB_STRING_DIR}/strsignal.c"
#     "${NEWLIB_STRING_DIR}/strspn.c"
#     "${NEWLIB_STRING_DIR}/strtok.c"
#     "${NEWLIB_STRING_DIR}/strtok_r.c"
#     "${NEWLIB_STRING_DIR}/strupr.c"
#     "${NEWLIB_STRING_DIR}/strxfrm.c"
#     "${NEWLIB_STRING_DIR}/strstr.c"
#     "${NEWLIB_STRING_DIR}/swab.c"
#     "${NEWLIB_STRING_DIR}/timingsafe_bcmp.c"
#     "${NEWLIB_STRING_DIR}/timingsafe_memcmp.c"
# )

# # create newlib library.
# add_library(newlib 
#     ${NEWLIB_CTYPE}
#     ${NEWLIB_STRING}
#     ${NEWLIB_LIBM_COMMON}
#     ${NEWLIB_LIBM_MATH}
# )

###############################################################################
# libopencm3

set(LIBOPENCM3_DIR "${LIB_DIR}/libopencm3")
message("LIBOPENCM3_DIR: ${LIBOPENCM3_DIR}")

# clone libopencm3 into codal/libraries/codal-libopencm3/libopencm3
# TODO: "--branch" "${LIBOPENCM3_VERSION}" "--single-branch"
execute_process(
    WORKING_DIRECTORY "${LIB_DIR}" 
    COMMAND "git" "clone" "https://github.com/libopencm3/libopencm3" 
    OUTPUT_VARIABLE git_clone_libopencm3)

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
# RECURSIVE_FIND_FILE(LIBOPENCM3_CM3 "${LIBOPENCM3_DIR}/lib/cm3" "*.c")
set(
    LIBOPENCM3_CM3
    "${LIBOPENCM3_DIR}/lib/cm3/nvic.c"
    "${LIBOPENCM3_DIR}/lib/cm3/vector.c"
)
RECURSIVE_FIND_FILE(LIBOPENCM3_COMMON "${LIBOPENCM3_DIR}/lib/stm32/common" "*.c")
# RECURSIVE_FIND_FILE(LIBOPENCM3_F1 "${LIBOPENCM3_DIR}/lib/stm32/f1" "*.c")
set(
    LIBOPENCM3_F1
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

# create libopencm3 library.
add_library(libopencm3 
    ${LIBOPENCM3_CM3}
    ${LIBOPENCM3_COMMON}
    ${LIBOPENCM3_F1}
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
# RECURSIVE_FIND_FILE(TOP_LEVEL_SOURCE_FILES "${CODAL_LIBOPENCM3_DIR}" "*.cpp")
set(
    TOP_LEVEL_SOURCE_FILES
    "${CODAL_LIBOPENCM3_DIR}/sensor_display.cpp"
)

# create our codal-libopencm3 target.
add_library(codal-libopencm3 ${TOP_LEVEL_SOURCE_FILES})
message("TOP_LEVEL_SOURCE_FILES: ${TOP_LEVEL_SOURCE_FILES}")

###############################################################################
# global settings

string(TOLOWER "${CODAL_TARGET_PROCESSOR}" TARGET_PROCESSOR)

# linker addresses
set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -T\"${CMAKE_CURRENT_LIST_DIR}/ld/stm32f103x8.ld\"" PARENT_SCOPE)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -T\"${CMAKE_CURRENT_LIST_DIR}/ld/stm32f103x8.ld\"" PARENT_SCOPE)
set(CMAKE_SYSTEM_PROCESSOR "armv7-m" PARENT_SCOPE)

# include directories
include_directories(    
    ${INCLUDE_DIRS} 
    ${TOP_LEVEL_INCLUDE_DIRS}
    ${CODAL_CORE_INC}
    ${COCOOS_INC}
    ${BME280_INC}
    ${BOOST_INC}
    ${STM32_INC}
    ${LIBOPENCM3_INC}
    ${LIB_DIR}
)

# libraries to be built
target_link_libraries(
    codal-libopencm3
    codal-core
    cocoos
    bme280
    stm32
    libopencm3
    newlib
)

# expose it to parent cmake.
target_include_directories(codal-libopencm3 PUBLIC ${MODEL_INCLUDE_DIRS} ${TOP_LEVEL_INCLUDE_DIRS})
