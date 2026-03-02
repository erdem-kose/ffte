#!/bin/bash
# Copyright (c) [2022-2026] Erdem Kose

# Clones and builds GoogleTest locally into lib/external/googletest/.
# Equivalent of install_googletest.ps1 on Windows.

set -e

cwd="$(dirname "$(realpath "$0")")"
rundir="$(pwd)"

export PATH="$cwd/external/gcc/bin:$cwd/external/cmake/bin:$PATH"

path="$cwd/external/googletest"

git clone --branch v1.17.0 --single-branch https://github.com/google/googletest.git "$path"

rm -rf "$path/build"
mkdir -p "$path/build"

cd "$path/build"

cmake .. -DBUILD_GMOCK=OFF \
         -G "Unix Makefiles" \
         -DCMAKE_TRY_COMPILE_TARGET_TYPE=STATIC_LIBRARY \
         -DCMAKE_INSTALL_PREFIX="$path/install"

make
make install

cd "$rundir"

# Move install contents up and remove build artifacts
find "$path" -maxdepth 1 ! -name "install" ! -name "$(basename "$path")" -exec rm -rf {} +
mv "$path/install/"* "$path/"
rm -rf "$path/install"
