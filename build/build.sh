#!/usr/bin/env bash

set -e

# === CONFIG ===
PROJECT_NAME="lulu"
OUTPUT_DIR="build"
CXX="g++"
STD="-std=c++20"
FLAGS="-Wall -Wextra -O2"

# === EXPLICIT SOURCES ===
SOURCES=(
    "core/main.cpp"
    "core/src/*.cpp"
    "core/src/actors/*.cpp"
)

# === INCLUDES ===
INCLUDES="-Icore/include"

# === RAYLIB ===
RAYLIB_FLAGS=$(pkg-config --cflags --libs raylib)

# === COMPILE ===
mkdir -p "$OUTPUT_DIR"

echo "Compiling project: $PROJECT_NAME"
echo "Sources: ${SOURCES[*]}"

$CXX $STD $FLAGS $INCLUDES ${SOURCES[*]} $RAYLIB_FLAGS -o "$OUTPUT_DIR/$PROJECT_NAME"

echo "✅ Build successful! Output: $OUTPUT_DIR/$PROJECT_NAME"