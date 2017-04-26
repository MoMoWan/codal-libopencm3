project(codal-mbed)

include(../../utils/cmake/util.cmake)

RECURSIVE_FIND_DIR(INCLUDE_DIRS "inc" "*.h")
RECURSIVE_FIND_FILE(SOURCE_FILES "source" "*.c??")

include_directories("${INCLUDE_DIRS}")

add_library(codal-mbed "${SOURCE_FILES}" )

target_link_libraries(
    codal-mbed
    codal
    mbed-classic
)

target_include_directories(codal-mbed PUBLIC ${INCLUDE_DIRS})

if(CODAL_HEADER_EXTRACTION)
    RECURSIVE_FIND_FILE(INCLUDE_FILES "inc" "*.h")
    file(COPY ${INCLUDE_FILES} DESTINATION "../../build/codal-mbed/includes")
endif()
