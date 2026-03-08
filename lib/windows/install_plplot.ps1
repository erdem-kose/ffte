# Copyright (c) [2022-2026] Erdem Kose

cls

$ErrorActionPreference = "Stop"

$cwd = Split-Path -Path $PSCommandPath
$cwd = $cwd -replace '\\', '/'
$rundir = Get-Location

$env:Path = "$cwd/../external/mingw/bin;$cwd/../external/cmake/bin;$env:Path"

$external = "$cwd/../external"
$zlibDir = "$external/zlib"
$pngDir  = "$external/libpng"
$gdDir  = "$external/libgd"
$plplotPath = "$external/plplot"

### --- 1. Clone and build static zlib ---
if (Test-Path $zlibDir) { Remove-Item $zlibDir -Recurse -Force }
git clone --branch v1.3.1 --single-branch https://github.com/madler/zlib.git $zlibDir

if (Test-Path "$zlibDir/build") { Remove-Item "$zlibDir/build" -Recurse -Force }
New-Item -ItemType Directory -Path "$zlibDir/build" | Out-Null
Set-Location "$zlibDir/build"

cmake .. -G "MinGW Makefiles" `
    -DCMAKE_INSTALL_PREFIX="$zlibDir/installfolder" `
    -DBUILD_SHARED_LIBS=OFF

make
make install

### --- 2. Clone and build static libpng ---
if (Test-Path $pngDir) { Remove-Item $pngDir -Recurse -Force }
git clone --branch libpng16 --single-branch https://git.code.sf.net/p/libpng/code $pngDir

if (Test-Path "$pngDir/build") { Remove-Item "$pngDir/build" -Recurse -Force }
New-Item -ItemType Directory -Path "$pngDir/build" | Out-Null
Set-Location "$pngDir/build"

cmake .. -G "MinGW Makefiles" `
    -DCMAKE_INSTALL_PREFIX="$pngDir/installfolder" `
    -DPNG_SHARED=OFF `
    -DPNG_STATIC=ON `
    -DZLIB_LIBRARY="$zlibDir/installfolder/lib/libzlibstatic.a" `
    -DZLIB_INCLUDE_DIR="$zlibDir/installfolder/include" `
    -DBUILD_SHARED_LIBS=OFF

make
make install

### --- 3. Clone and build static libgd ---
if (Test-Path $gdDir) { Remove-Item $gdDir -Recurse -Force }
git clone --branch gd-2.3.3 --single-branch https://github.com/libgd/libgd.git $gdDir

if (Test-Path "$gdDir/build") { Remove-Item "$gdDir/build" -Recurse -Force }
New-Item -ItemType Directory -Path "$gdDir/build" | Out-Null
Set-Location "$gdDir/build"

cmake .. -G "MinGW Makefiles" `
    -DCMAKE_INSTALL_PREFIX="$gdDir/installfolder" `
    -DBUILD_SHARED_LIBS=OFF `
    -DBUILD_STATIC_LIBS=ON `
    -DENABLE_PNG=ON `
    -DZLIB_INCLUDE_DIR="$zlibDir/installfolder/include" `
    -DZLIB_LIBRARY="$zlibDir/installfolder/lib/libzlibstatic.a" `
    -DPNG_PNG_INCLUDE_DIR="$pngDir/installfolder/include" `
    -DPNG_LIBRARY="$pngDir/installfolder/lib/libpng.a" `
    -DWIN32=ON

make
make install

### --- 3.5. Patch gd.h to force static declarations (suppress dllimport on Windows) ---
# libgd 2.3.3 uses BGDWIN32+NONDLL (not BGD_STATIC) to select the static path.
# Without BGDWIN32 defined, gd.h always falls into the dllimport branch on _WIN32.
$gdHeader = "$gdDir/installfolder/include/gd.h"
$content = "#ifndef BGDWIN32`n#define BGDWIN32`n#endif`n#ifndef NONDLL`n#define NONDLL`n#endif`n" + (Get-Content $gdHeader -Raw)
Set-Content $gdHeader $content -NoNewline

### --- 4. Clone and build PLplot statically with only the PNG driver ---
if (Test-Path "$plplotPath") { Remove-Item "$plplotPath" -Recurse -Force }
git clone --branch plplot-5.15.0 --single-branch https://git.code.sf.net/p/plplot/plplot "$plplotPath"

if (Test-Path "$plplotPath/build") { Remove-Item "$plplotPath/build" -Recurse -Force }
New-Item -ItemType Directory -Path "$plplotPath/build" | Out-Null
Set-Location "$plplotPath/build"

cmake .. -G "MinGW Makefiles" `
    -DCMAKE_INSTALL_PREFIX="$plplotPath/installfolder/" `
    -DBUILD_SHARED_LIBS=OFF `
    -DCMAKE_C_FLAGS="-DBGDWIN32 -DNONDLL" `
    -DPLD_png=ON `
    -DPLD_pdf=OFF `
    -DPLD_svg=ON `
    -DPLD_qt=OFF `
    -DPLD_qtwidget=OFF `
    -DPLD_xwin=OFF `
    -DPLD_tk=OFF `
    -DPLD_ntk=OFF `
    -DPLD_wxwidgets=OFF `
    -DENABLE_qt=OFF `
    -DENABLE_qt_gui=OFF `
    -DENABLE_tcl=OFF `
    -DENABLE_tk=OFF `
    -DENABLE_ntk=OFF `
    -DENABLE_itcl=OFF `
    -DENABLE_itk=OFF `
    -DENABLE_pstex=OFF `
    -DENABLE_DYNDRIVERS=OFF `
    -DENABLE_STATIC_DRIVERS=ON `
    -DENABLE_qsastime=ON `
    -DCMAKE_SKIP_INSTALL_RPATH=ON `
    -DCMAKE_BUILD_RPATH_USE_ORIGIN=ON `
    -DGD_INCLUDE_DIR="$gdDir/installfolder/include" `
    -DGD_LIBRARY="$gdDir/installfolder/lib/libgd.a" `
    -DGD_LIBRARIES="$gdDir/installfolder/lib/libgd.a;$pngDir/installfolder/lib/libpng.a;$zlibDir/installfolder/lib/libzlibstatic.a" `
    -DPNG_PNG_INCLUDE_DIR="$pngDir/installfolder/include" `
    -DPNG_LIBRARY="$pngDir/installfolder/lib/libpng.a" `
    -DZLIB_INCLUDE_DIR="$zlibDir/installfolder/include" `
    -DZLIB_LIBRARY="$zlibDir/installfolder/lib/libzlibstatic.a"

make
make install
### --- 4. Final cleanup ---
Set-Location -Path "$rundir"