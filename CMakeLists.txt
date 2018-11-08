project(codal-libopencm3)

include("${CODAL_UTILS_LOCATION}")

# CMAKE_CURRENT_SOURCE_DIR is codal/libraries/codal-libopencm3
message("CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")

# build the libraries in the "lib" folder.
set(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib")
message("LIB_DIR: ${LIB_DIR}")

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

# find stm32 sources and headers.
RECURSIVE_FIND_DIR(STM32_INC "${STM32_DIR}" "*.h")
RECURSIVE_FIND_FILE(STM32_SRC "${STM32_DIR}" "*.cpp")

# create stm32 library.
add_library(stm32 ${STM32_SRC})

###############################################################################
# libopencm3

set(LIBOPENCM3_DIR "${LIB_DIR}/libopencm3")
message("LIBOPENCM3_DIR: ${LIBOPENCM3_DIR}")

# clone libopencm3 into codal/libraries/codal-libopencm3/libopencm3
execute_process(WORKING_DIRECTORY "${LIB_DIR}" COMMAND "git" "clone" "https://github.com/libopencm3/libopencm3" OUTPUT_VARIABLE git_clone_libopencm3)

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
RECURSIVE_FIND_FILE(LIBOPENCM3_COMMON "${LIBOPENCM3_DIR}/lib/stm32/common" "*.c")
RECURSIVE_FIND_FILE(LIBOPENCM3_F1 "${LIBOPENCM3_DIR}/lib/stm32/f1" "*.c")

# create libopencm3 library.
add_library(libopencm3 
    ${LIBOPENCM3_COMMON}
    ${LIBOPENCM3_F1}
)

###############################################################################
# codal-libopencm3

# find codal-libopencm3 sources and headers.
set(TOP_LEVEL_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src")
RECURSIVE_FIND_FILE(TOP_LEVEL_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src" "*.cpp")
LIST(APPEND TOP_LEVEL_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/asm/CortexContextSwitch.s")

# create our codal-libopencm3 target.
add_library(codal-libopencm3 ${TOP_LEVEL_SOURCE_FILES})

###############################################################################
# global settings

string(TOLOWER "${CODAL_TARGET_PROCESSOR}" TARGET_PROCESSOR)

set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -T\"${CMAKE_CURRENT_LIST_DIR}/ld/stm32f103x8.ld\"" PARENT_SCOPE)
set(CMAKE_EXE_LINKER_FLAGS    "${CMAKE_EXE_LINKER_FLAGS} -T\"${CMAKE_CURRENT_LIST_DIR}/ld/stm32f103x8.ld\"" PARENT_SCOPE)
set(CMAKE_SYSTEM_PROCESSOR "armv7-m" PARENT_SCOPE)

# include directories
include_directories(
    ${INCLUDE_DIRS} 
    ${TOP_LEVEL_INCLUDE_DIRS}
    ${COCOOS_INC}
    ${BME280_INC}
    ${BOOST_INC}
    ${STM32_INC}
    ${LIBOPENCM3_INC}
)

# libraries to be built
target_link_libraries(
    codal-libopencm3
    codal-core
    cocoos
    bme280
    stm32
    libopencm3
)

# expose it to parent cmake.
target_include_directories(codal-libopencm3 PUBLIC ${TOP_LEVEL_INCLUDE_DIRS})
