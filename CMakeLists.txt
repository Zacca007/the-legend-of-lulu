cmake_minimum_required(VERSION 3.16)
project(lulu)

# === CONFIG ===
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O2")

# === FIND RAYLIB ===
find_package(PkgConfig REQUIRED)
pkg_check_modules(RAYLIB REQUIRED raylib)

# === SOURCES ===
file(GLOB_RECURSE SOURCES
    "core/main.cpp"
    "core/src/*.cpp"
    "core/src/actors/*.cpp"
)

# === CREATE EXECUTABLE ===
add_executable(${PROJECT_NAME} ${SOURCES}
        core/include/animationHandler.hpp
        core/src/animationHandler.cpp
        core/src/movable.cpp
        core/include/actors/trapBlade.hpp
        core/src/actors/trapBlade.cpp)

# === INCLUDES ===
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/core/include
)

# === LINK RAYLIB ===
target_link_libraries(${PROJECT_NAME} ${RAYLIB_LIBRARIES})
target_include_directories(${PROJECT_NAME} PRIVATE ${RAYLIB_INCLUDE_DIRS})
target_compile_options(${PROJECT_NAME} PRIVATE ${RAYLIB_CFLAGS_OTHER})

# === OUTPUT DIRECTORY ===
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)
