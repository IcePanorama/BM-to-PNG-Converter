cmake_minimum_required (VERSION 3.28)

project(BMtoPNG C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

set(WIN32_CC i686-w64-mingw32-gcc)
set(WIN64_CC x86_64-w64-mingw32-gcc)

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

set(BIN_DIR "${PROJECT_SOURCE_DIR}/bin")
set(HEADER_CPY_DIR "${BIN_DIR}/include/${PROJECT_NAME}")

set(INCL_DIR "${PROJECT_SOURCE_DIR}/include")
set(WIN32_INCL_DIR "${PROJECT_SOURCE_DIR}/include/win32")
set(WIN64_INCL_DIR "${PROJECT_SOURCE_DIR}/include/win64")
set(INCL_FILE "${INCL_DIR}/bm_to_png_converter.h")

set(LIB_DIR "${PROJECT_SOURCE_DIR}/lib")
set(WIN32_LIB_DIR "${PROJECT_SOURCE_DIR}/lib/win32")
set(WIN64_LIB_DIR "${PROJECT_SOURCE_DIR}/lib/win64")
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

# Copy target
add_custom_target(copy
    COMMAND ${CMAKE_COMMAND} -E copy ${INCL_FILE} ${WIN32_INCL_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy ${INCL_FILE} ${WIN64_INCL_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy ${INCL_FILE} ${PROJECT_SOURCE_DIR}
    COMMENT "Copying ${INCL_FILE} to win32/win64 include directories and project root."
)

# Default/All target
add_executable(${PROJECT_NAME} ${SRC_FILES})
add_dependencies(${PROJECT_NAME} format)
add_dependencies(${PROJECT_NAME} copy)
target_include_directories(${PROJECT_NAME} PRIVATE ${INCL_DIR})
target_link_libraries(${PROJECT_NAME} -lraylib -lm -lGL)

# Win32 target
add_custom_target(win32
    COMMAND ${WIN32_CC} ${CMAKE_C_FLAGS} -o "${PROJECT_NAME}_i686.exe" "${SRC_DIR}/*.c" -I${WIN32_INCL_DIR} -L${WIN32_LIB_DIR} -lraylib -lopengl32 -lgdi32 -lwinmm
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    COMMENT "Building 32-bit executable."
)

# Win64 target
add_custom_target(win64
    COMMAND ${WIN64_CC} ${CMAKE_C_FLAGS} -o "${PROJECT_NAME}_x86_64.exe" "${SRC_DIR}/*.c" -I${WIN64_INCL_DIR} -L${WIN64_LIB_DIR} -lraylib -lopengl32 -lgdi32 -lwinmm
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    COMMENT "Building 64-bit executable."
)

# Release target
add_custom_target(release
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target clean
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target ${PROJECT_NAME}
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target win32
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target win64
    COMMENT "Running full build (clean, format, build, and test)."
)

# Clean target
set(extra_files_to_clean
    "${PROJECT_SOURCE_DIR}/${PROJECT_NAME}_i686.exe"
    "${PROJECT_SOURCE_DIR}/${PROJECT_NAME}_x86_64.exe"
)
set_property(
	TARGET ${PROJECT_NAME}
	APPEND
	PROPERTY ADDITIONAL_CLEAN_FILES ${extra_files_to_clean}
)
