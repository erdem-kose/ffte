cls
$ErrorActionPreference = "Stop"

$cwd = Split-Path -Path $PSCommandPath
$cwd = $cwd -replace '\\', '/'
$rundir = Get-Location

$env:Path = "$cwd/external/mingw/bin;$cwd/external/cmake/bin;$env:Path"

$external = "$cwd/external"
$zlibDir = "$external/zlib"
$pngDir  = "$external/libpng"
$plplotPath = "$external/plplot"

### --- 1. Clone and build static zlib ---
if (Test-Path $zlibDir) { Remove-Item $zlibDir -Recurse -Force }
git clone --branch v1.3.1 --single-branch https://github.com/madler/zlib.git $zlibDir

if (Test-Path "$zlibDir/build") { Remove-Item "$zlibDir/build" -Recurse -Force }
New-Item -ItemType Directory -Path "$zlibDir/build" | Out-Null
Set-Location "$zlibDir/build"

cmake .. -G "MinGW Makefiles" `
    -DCMAKE_INSTALL_PREFIX="$zlibDir/build/" `
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
    -DCMAKE_INSTALL_PREFIX="$pngDir/build/" `
    -DPNG_SHARED=OFF `
    -DPNG_STATIC=ON `
    -DZLIB_LIBRARY="$zlibDir/build/lib/libz.a" `
    -DZLIB_INCLUDE_DIR="$zlibDir/build/include" `
    -DBUILD_SHARED_LIBS=OFF

make
make install

### --- 3. Clone and build PLplot statically with PNG only ---
if (Test-Path $plplotPath) { Remove-Item $plplotPath -Recurse -Force }
git clone --branch plplot-5.15.0 --single-branch https://git.code.sf.net/p/plplot/plplot $plplotPath

if (Test-Path "$plplotPath/build") { Remove-Item "$plplotPath/build" -Recurse -Force }
New-Item -ItemType Directory -Path "$plplotPath/build" | Out-Null
Set-Location "$plplotPath/build"

cmake .. -G "MinGW Makefiles" `
    -DCMAKE_INSTALL_PREFIX="$plplotPath/build/" `
    -DBUILD_SHARED_LIBS=OFF `
    -DPLD_png=ON `
    -DPLD_pdf=OFF `
    -DPLD_svg=OFF `
    -DPLD_qt=OFF `
    -DPLD_qtwidget=OFF `
    -DPLD_xwin=OFF `
    -DPLD_tk=OFF `
    -DPLD_ntk=OFF `
    -DPLD_wxwidgets=OFF `
    -DENABLE_qt=OFF `
    -DENABLE_qt_gui=OFF `
    -DENABLE_tk=OFF `
    -DENABLE_ntk=OFF `
    -DENABLE_itcl=OFF `
    -DENABLE_itk=OFF `
    -DENABLE_pstex=OFF `
    -DENABLE_DYNDRIVERS=OFF `
    -DENABLE_STATIC_DRIVERS=ON `
    -DENABLE_qsastime=ON `
    -DCMAKE_PREFIX_PATH="$pngDir/build/;$zlibDir/build/" `
    -DCMAKE_C_FLAGS="-I$pngDir/build/include -I$zlibDir/build/include" `
    -DCMAKE_EXE_LINKER_FLAGS="-L$pngDir/build/lib -L$zlibDir/build/lib -static"

make
make install

### --- 4. Final cleanup ---
Set-Location -Path "$rundir"
