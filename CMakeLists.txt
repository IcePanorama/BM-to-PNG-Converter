cmake_minimum_required (VERSION 3.28)

project(BMtoPNG C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

add_compile_options(
    -std=c99
    -Wpedantic
    -Wextra
    -Werror
    -Wall
    -Wstrict-aliasing=3
    -Wwrite-strings
    -Wvla
    -Wcast-align=strict
    -Wstrict-prototypes
    -Wstringop-overflow=4
    -Wshadow
    -fanalyzer
)

set(AUTO_FMT clang-format)
set(CODE_STYLE GNU)

set(INCL_DIR "${PROJECT_SOURCE_DIR}/include")
set(LIB_DIR "${PROJECT_SOURCE_DIR}/lib")
set(RAYLIB_LIB "${LIB_DIR}/libraylib.a")

set(SRC_DIR "${PROJECT_SOURCE_DIR}/src")
set(SRC_FILES
    "${SRC_DIR}/main.c"
)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})

# Format target
add_custom_target(format
    COMMAND ${AUTO_FMT} -style=${CODE_STYLE} -i ${SRC_FILES} "${INCL_DIR}/*.h"
    COMMENT "Auto-formatting code."
)

# Test target
add_custom_target(test
    COMMAND valgrind --leak-check=yes ./${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/dat-files/HARVEST.DAT
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Testing for memory leaks with Valgrind."
)

# Default/All target
add_executable(${PROJECT_NAME} ${SRC_FILES})
add_dependencies(${PROJECT_NAME} format)
target_include_directories(${PROJECT_NAME} PRIVATE ${INCL_DIR})
target_link_libraries(${PROJECT_NAME} ${RAYLIB_LIB} -lm -lGL)

# `full` target
add_custom_target(full
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target clean
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target ${PROJECT_NAME}
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target test
    COMMENT "Running full build (clean, format, build, and test)."
)
