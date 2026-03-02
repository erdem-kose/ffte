#!/bin/bash
# Copyright (c) [2022-2026] Erdem Kose

# Downloads a local CMake binary into lib/external/cmake/.
# Equivalent of install_cmake.ps1 on Windows.

set -e

cwd="$(dirname "$(realpath "$0")")"

cmake_version="3.31.5"

URL="https://github.com/Kitware/CMake/releases/download/v${cmake_version}/cmake-${cmake_version}-linux-x86_64.tar.gz"

path="$cwd/../external"
archive="$path/cmake-${cmake_version}-linux-x86_64.tar.gz"

mkdir -p "$path"

# Download the archive
curl -L -o "$archive" "$URL"

# Extract and rename to a fixed directory name
tar -xzf "$archive" -C "$path"
mv "$path/cmake-${cmake_version}-linux-x86_64" "$path/cmake"

# Clean up archive
rm -f "$archive"
