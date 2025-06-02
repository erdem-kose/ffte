cls

$cwd=Split-Path -Path $PSCommandPath 
$cwd=$cwd -replace '\\', '/'

$rundir = Get-Location

$env:Path = "$cwd/external/mingw/bin;$cwd/external/cmake/bin;$env:Path"

$Path = "$cwd/external/googletest"

git clone --branch v1.17.0 --single-branch https://github.com/google/googletest.git $Path

if (Test-Path "$Path/build") { Remove-Item -Path "$Path/build" -Recurse -Force }
New-Item -ItemType Directory -Path "$Path/build" -Force

Set-Location -Path "$Path/build"

cmake .. -DBUILD_GMOCK=OFF -G "MinGW Makefiles" -DCMAKE_TRY_COMPILE_TARGET_TYPE=STATIC_LIBRARY -DCMAKE_INSTALL_PREFIX="$Path/install"
make
make install

Set-Location -Path "$rundir"