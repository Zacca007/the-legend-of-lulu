#!/usr/bin/env bash

set -e

# === CONFIG ===
PROJECT_NAME="lulu"
SRC_DIRS=("core" "core/src")
INCLUDE_DIRS=("core/include")
OUTPUT_DIR="build"
CXX="g++"  # o clang++
STD="-std=c++20"
FLAGS=("-Wall" "-Wextra" "-O2")

# === GATHER SOURCES ===
SOURCES=()
for dir in "${SRC_DIRS[@]}"; do
    while IFS= read -r -d '' file; do
        SOURCES+=("$file")
    done < <(find "$dir" -name '*.cpp' -print0)
done

# === INCLUDE PATHS ===
INCLUDE_FLAGS=()
for dir in "${INCLUDE_DIRS[@]}"; do
    INCLUDE_FLAGS+=("-I$dir")
done

# === RAYLIB FLAGS ===
PKG_CONFIG_FLAGS=($(pkg-config --cflags --libs raylib))

# === COMPILE ===
mkdir -p "$OUTPUT_DIR"

echo "Compiling project: $PROJECT_NAME"
$CXX $STD "${FLAGS[@]}" "${INCLUDE_FLAGS[@]}" "${SOURCES[@]}" "${PKG_CONFIG_FLAGS[@]}" -o "$OUTPUT_DIR/$PROJECT_NAME"

echo "✅ Build successful! Output: $OUTPUT_DIR/$PROJECT_NAME"
