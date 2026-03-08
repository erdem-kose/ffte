#!/bin/bash
# Copyright (c) [2022-2026] Erdem Kose

# Builds zlib → libpng → libgd → PLplot from source into lib/external/.
# Equivalent of install_plplot.ps1 on Windows.

set -e

cwd="$(dirname "$(realpath "$0")")"
rundir="$(pwd)"

export PATH="$cwd/../external/gcc/bin:$cwd/../external/cmake/bin:$PATH"

external="$cwd/../external"
zlibDir="$external/zlib"
pngDir="$external/libpng"
gdDir="$external/libgd"
plplotPath="$external/plplot"

### --- 1. Clone and build static zlib ---
rm -rf "$zlibDir"
git clone --branch v1.3.1 --single-branch https://github.com/madler/zlib.git "$zlibDir"

rm -rf "$zlibDir/build"
mkdir -p "$zlibDir/build"
cd "$zlibDir/build"

cmake .. -G "Ninja" \
    -DCMAKE_MAKE_PROGRAM="$cwd/../external/gcc/bin/ninja" \
    -DCMAKE_C_COMPILER="$cwd/../external/gcc/bin/x86_64-buildroot-linux-gnu-gcc" \
    -DCMAKE_CXX_COMPILER="$cwd/../external/gcc/bin/x86_64-buildroot-linux-gnu-g++" \
    -DCMAKE_INSTALL_PREFIX="$zlibDir/installfolder" \
    -DBUILD_SHARED_LIBS=OFF

cmake --build .
cmake --install .
ln -s "$zlibDir/installfolder/lib/libz.a" "$zlibDir/installfolder/lib/libzlibstatic.a"

### --- 2. Clone and build static libpng ---
rm -rf "$pngDir"
git clone --branch libpng16 --single-branch https://git.code.sf.net/p/libpng/code "$pngDir"

rm -rf "$pngDir/build"
mkdir -p "$pngDir/build"
cd "$pngDir/build"

cmake .. -G "Ninja" \
    -DCMAKE_MAKE_PROGRAM="$cwd/../external/gcc/bin/ninja" \
    -DCMAKE_C_COMPILER="$cwd/../external/gcc/bin/x86_64-buildroot-linux-gnu-gcc" \
    -DCMAKE_CXX_COMPILER="$cwd/../external/gcc/bin/x86_64-buildroot-linux-gnu-g++" \
    -DCMAKE_INSTALL_PREFIX="$pngDir/installfolder" \
    -DPNG_SHARED=OFF \
    -DPNG_STATIC=ON \
    -DZLIB_LIBRARY="$zlibDir/installfolder/lib/libzlibstatic.a" \
    -DZLIB_INCLUDE_DIR="$zlibDir/installfolder/include" \
    -DBUILD_SHARED_LIBS=OFF

cmake --build .
cmake --install .

### --- 3. Clone and build static libgd ---
rm -rf "$gdDir"
git clone --branch gd-2.3.3 --single-branch https://github.com/libgd/libgd.git "$gdDir"

rm -rf "$gdDir/build"
mkdir -p "$gdDir/build"
cd "$gdDir/build"

cmake .. -G "Ninja" \
    -DCMAKE_MAKE_PROGRAM="$cwd/../external/gcc/bin/ninja" \
    -DCMAKE_C_COMPILER="$cwd/../external/gcc/bin/x86_64-buildroot-linux-gnu-gcc" \
    -DCMAKE_CXX_COMPILER="$cwd/../external/gcc/bin/x86_64-buildroot-linux-gnu-g++" \
    -DCMAKE_INSTALL_PREFIX="$gdDir/installfolder" \
    -DBUILD_SHARED_LIBS=OFF \
    -DBUILD_STATIC_LIBS=ON \
    -DENABLE_PNG=ON \
    -DZLIB_INCLUDE_DIR="$zlibDir/installfolder/include" \
    -DZLIB_LIBRARY="$zlibDir/installfolder/lib/libzlibstatic.a" \
    -DPNG_PNG_INCLUDE_DIR="$pngDir/installfolder/include" \
    -DPNG_LIBRARY="$pngDir/installfolder/lib/libpng.a"

cmake --build .
cmake --install .

### --- 4. Clone and build PLplot statically with only the PNG/SVG driver ---
rm -rf "$plplotPath"
git clone --branch plplot-5.15.0 --single-branch https://git.code.sf.net/p/plplot/plplot "$plplotPath"

rm -rf "$plplotPath/build"
mkdir -p "$plplotPath/build"
cd "$plplotPath/build"

gcc_sysroot="$($cwd/../external/gcc/bin/x86_64-buildroot-linux-gnu-gcc --print-sysroot)"

cmake .. -G "Ninja" \
    -DCMAKE_MAKE_PROGRAM="$cwd/../external/gcc/bin/ninja" \
    -DCMAKE_C_COMPILER="$cwd/../external/gcc/bin/x86_64-buildroot-linux-gnu-gcc" \
    -DCMAKE_CXX_COMPILER="$cwd/../external/gcc/bin/x86_64-buildroot-linux-gnu-g++" \
    -DCMAKE_SYSROOT="$gcc_sysroot" \
    -DCMAKE_REQUIRED_LIBRARIES="$pngDir/installfolder/lib/libpng.a;$zlibDir/installfolder/lib/libzlibstatic.a;m" \
    -DCMAKE_INSTALL_PREFIX="$plplotPath/installfolder/" \
    -DBUILD_SHARED_LIBS=OFF \
    -DPLD_png=ON \
    -DPLD_pdf=OFF \
    -DPLD_svg=ON \
    -DPLD_qt=OFF \
    -DPLD_qtwidget=OFF \
    -DPLD_xwin=OFF \
    -DPLD_tk=OFF \
    -DPLD_ntk=OFF \
    -DPLD_wxwidgets=OFF \
    -DENABLE_qt=OFF \
    -DENABLE_qt_gui=OFF \
    -DENABLE_tcl=OFF \
    -DENABLE_tk=OFF \
    -DENABLE_ntk=OFF \
    -DENABLE_itcl=OFF \
    -DENABLE_itk=OFF \
    -DENABLE_pstex=OFF \
    -DENABLE_DYNDRIVERS=OFF \
    -DENABLE_STATIC_DRIVERS=ON \
    -DENABLE_qsastime=ON \
    -DCMAKE_SKIP_INSTALL_RPATH=ON \
    -DCMAKE_BUILD_RPATH_USE_ORIGIN=ON \
    -DGD_INCLUDE_DIR="$gdDir/installfolder/include" \
    -DGD_LIBRARY="$gdDir/installfolder/lib/libgd.a" \
    -DGD_LIBRARIES="$gdDir/installfolder/lib/libgd.a;$pngDir/installfolder/lib/libpng.a;$zlibDir/installfolder/lib/libzlibstatic.a" \
    -DPNG_PNG_INCLUDE_DIR="$pngDir/installfolder/include" \
    -DPNG_LIBRARY="$pngDir/installfolder/lib/libpng.a" \
    -DZLIB_INCLUDE_DIR="$zlibDir/installfolder/include" \
    -DZLIB_LIBRARY="$zlibDir/installfolder/lib/libzlibstatic.a"

cmake --build .
cmake --install .

### --- 5. Final cleanup ---
cd "$rundir"
