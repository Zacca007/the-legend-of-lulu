cmake_minimum_required(VERSION 3.22)
project(legend_of_lulu LANGUAGES CXX)

# === CONFIG ===
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# === FIND RAYLIB ===
find_package(PkgConfig REQUIRED)
pkg_check_modules(RAYLIB REQUIRED raylib)

# === SOURCES ===
file(GLOB_RECURSE SRC_FILES ${CMAKE_SOURCE_DIR}/core/src/*.cpp)
file(GLOB_RECURSE HEADER_FILES ${CMAKE_SOURCE_DIR}/core/include/*.hpp)
set(MAIN_FILE ${CMAKE_SOURCE_DIR}/core/main.cpp)

# === TARGET ===
add_executable(${PROJECT_NAME}
        ${MAIN_FILE}
        ${SRC_FILES}
        ${HEADER_FILES} # opzionale
)

# === INCLUDE PATHS ===
target_include_directories(${PROJECT_NAME}
        PRIVATE
        ${CMAKE_SOURCE_DIR}/core/include
        ${RAYLIB_INCLUDE_DIRS}
)

# === LINK LIBRARIES ===
target_link_libraries(${PROJECT_NAME}
        PRIVATE ${RAYLIB_LIBRARIES}
)

# === COMPILER FLAGS ===
target_compile_options(${PROJECT_NAME} PRIVATE
        -Wall
        -Wextra
        -pedantic
        ${RAYLIB_CFLAGS_OTHER}
)

# Ottimizzazioni per tipo di build
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_options(${PROJECT_NAME} PRIVATE -O3 -march=native -flto -DNDEBUG)
elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    target_compile_options(${PROJECT_NAME} PRIVATE -O2 -g -DNDEBUG)
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(${PROJECT_NAME} PRIVATE -O0 -g -DDEBUG)
endif()

# === OUTPUT DIRECTORY ===
set_target_properties(${PROJECT_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
