project(codal-libopencm3)

include("${CODAL_UTILS_LOCATION}")

# CMAKE_CURRENT_SOURCE_DIR is codal/libraries/codal-libopencm3
message("CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")

set(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib")
message("LIB_DIR: ${LIB_DIR}")

# clone libopencm3 into codal/libraries/codal-libopencm3/libopencm3
set(LIBOPENCM3_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib/libopencm3")
message("LIBOPENCM3_DIR: ${LIBOPENCM3_DIR}")
execute_process(WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib" COMMAND "git" "clone" "https://github.com/libopencm3/libopencm3" OUTPUT_VARIABLE git_clone_libopencm3)

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
RECURSIVE_FIND_DIR(LIBOPENCM3_INCLUDE_DIRS "${LIBOPENCM3_DIR}/include/stm32" "*.h")
RECURSIVE_FIND_FILE(LIBOPENCM3_COMMON "${LIBOPENCM3_DIR}/lib/stm32/common" "*.c")
RECURSIVE_FIND_FILE(LIBOPENCM3_F1 "${LIBOPENCM3_DIR}/lib/stm32/f1" "*.c")

# find sources and headers.
RECURSIVE_FIND_DIR(TOP_LEVEL_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src" "*.h")
RECURSIVE_FIND_FILE(TOP_LEVEL_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src" "*.c??")

LIST(APPEND TOP_LEVEL_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/asm/CortexContextSwitch.s")

RECURSIVE_FIND_DIR(LIB_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/lib" "*.h")
RECURSIVE_FIND_FILE(LIB_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/lib" "*.c??")
# message("LIB_SOURCE_FILES: ${LIB_SOURCE_FILES}")

RECURSIVE_FIND_DIR(STM32_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/stm32" "*.h")
RECURSIVE_FIND_FILE(STM32_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/stm32" "*.c??")
# message("STM32_SOURCE_FILES: ${STM32_SOURCE_FILES}")

string(TOLOWER "${CODAL_TARGET_PROCESSOR}" TARGET_PROCESSOR)

set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -T\"${CMAKE_CURRENT_LIST_DIR}/ld/stm32f103x8.ld\"" PARENT_SCOPE)
set(CMAKE_EXE_LINKER_FLAGS    "${CMAKE_EXE_LINKER_FLAGS} -T\"${CMAKE_CURRENT_LIST_DIR}/ld/stm32f103x8.ld\"" PARENT_SCOPE)
set(CMAKE_SYSTEM_PROCESSOR "armv7-m" PARENT_SCOPE)

# add them
include_directories(
    ${INCLUDE_DIRS} ${TOP_LEVEL_INCLUDE_DIRS} ${LIB_INCLUDE_DIRS} ${STM32_INCLUDE_DIRS} 
    "${LIBOPENCM3_DIR}/include"
    "${LIB_DIR}/boost_type_traits/src" 
    "${LIB_DIR}/boost_assert/src" 
    "${LIB_DIR}/boost_core/src" 
    "${LIB_DIR}/boost_utility/src" 
    "${LIB_DIR}/boost_predef/src" 
    "${LIB_DIR}/boost_mpl/src" 
    "${LIB_DIR}/boost_detail/src" 
    "${LIB_DIR}/boost_parameter/src" 
    "${LIB_DIR}/boost_iterator/src" 
    "${LIB_DIR}/boost_lockfree/src" 
)

# create libopencm3 library.
add_library(libopencm3 
    ${LIBOPENCM3_COMMON}
    ${LIBOPENCM3_F1}
)

# create our target
add_library(codal-libopencm3 ${TOP_LEVEL_SOURCE_FILES} ${LIB_SOURCE_FILES} ${STM32_SOURCE_FILES})

target_link_libraries(
    codal-libopencm3
    codal-core
    libopencm3
)

# expose it to parent cmake.
target_include_directories(codal-libopencm3 PUBLIC ${STM32_INCLUDE_DIRS} ${LIB_INCLUDE_DIRS} ${TOP_LEVEL_INCLUDE_DIRS})
