# ffte
Fast Fourier Transform for Embedded Systems in C and CPP. CPP has also Dynamic Fixed Point Number support (DynFix).
 * Power of 2 FFT's calculated by Cooley-Tukey
 * Non-power of 2 FFT's calculated by Bluestein
 * 2D FFT supported

## Usage in Your Project
The library is standalone — no external dependencies. Just copy the files under `src/` into your project and include the relevant headers:
 * `src/c/ffte/` — for the C implementation
 * `src/cpp/ffte/` — for the C++ implementation
 * `src/cpp/ffte_dynfix/` — for the C++ DynFix implementation

> **Note:** DynFix is a software-only dynamic fixed-point implementation. It is **not** hardware-accelerated and runs slower than the floating-point version. It exists purely as a showcase — to demonstrate how the FFT algorithm behaves with dynamic fixed-point arithmetic. It is not intended for production embedded deployment. To adapt it for real hardware, replace `DynFix<Tf,Ti>` with your platform's native fixed-point type or SIMD intrinsics and map the arithmetic operators accordingly — the algorithm structure remains the same.

The `tests/`, `utils/`, `lib/`, and `matlab/` folders are only needed for development and testing of this repo.

## Install requirements (Windows)
**Supported: Windows 10 / Windows 11, x86-64 only**
(PowerShell 5.1 is required; `Expand-Archive` and modern TLS are not available on Windows 8.1 or earlier.)

To install required software, please follow (you may need to disable antivirus for these, your responsibility!)

```powershell
lib/windows/install_mingw.ps1
lib/windows/install_cmake.ps1
lib/windows/install_googletest.ps1
lib/windows/install_plplot.ps1
```

All of these will be local installations under `lib/external`. So you don't need to worry if it messes with your local installations.
Scripts must be run in order: MinGW provides the compiler used by the remaining three scripts.

## Install requirements (Linux)
**Supported: Ubuntu 20.04+ (or any distro with glibc ≥ 2.27), x86-64 only**

To install required software, please follow

```bash
bash lib/linux/install_gcc.sh
bash lib/linux/install_cmake.sh
bash lib/linux/install_googletest.sh
bash lib/linux/install_plplot.sh
```

All of these will be local installations under `lib/external`. So you don't need to worry if it messes with your local installations.
Scripts must be run in order: GCC provides the compiler used by the remaining three scripts.

## Project Structure

```
ffte/
├── src/
│   ├── c/ffte/             # C implementation
│   └── cpp/
│       ├── ffte/           # C++ implementation
│       └── ffte_dynfix/    # C++ with Dynamic Fixed-Point (DynFix)
├── tests/
│   ├── ffte_c.cpp          # Google Test suite for C implementation
│   ├── ffte_cpp.cpp        # Google Test suite for C++ implementation
│   ├── ffte_cpp_dynfix.cpp # Google Test suite for DynFix implementation
│   └── output/
│       ├── c/              # SVG plots and times.txt (pow2 / nonpow2)
│       ├── cpp/            # SVG plots and times.txt (pow2 / nonpow2)
│       └── cpp_dynfix/     # SVG plots and times.txt (pow2 / nonpow2)
├── utils/                  # Shared utility functions (e.g. plotting)
├── lib/
│   ├── windows/            # Windows install scripts (PowerShell)
│   ├── linux/              # Linux install scripts (Bash)
│   └── external/           # Local toolchain and library installations
├── matlab/
│   ├── fft_bluestein/      # MATLAB reference: Bluestein FFT
│   └── lu_factorization/   # MATLAB reference: Doolittle LU factorization
└── .vscode/                # VSCode configuration (CMake kits, settings)
```

## Windows Defender False Positives
MinGW-compiled binaries may be flagged by Windows Defender (heuristic detections). To fix, add the build output folder to exclusions: **Windows Security → Virus & threat protection → Manage settings → Exclusions → Add or remove exclusions → Add Folder**. Your responsibility!

## Build and Test (Command Line)

**Windows** (from repo root, PowerShell or cmd):
```powershell
lib/external/cmake/bin/cmake.exe -G "MinGW Makefiles" -DCMAKE_C_COMPILER=lib/external/mingw/bin/gcc.exe -DCMAKE_CXX_COMPILER=lib/external/mingw/bin/g++.exe -S . -B build
lib/external/cmake/bin/cmake.exe --build build
build/ffte.exe
```
Or run tests via CTest:
```powershell
lib/external/cmake/bin/ctest.exe --test-dir build
```

**Linux** (from repo root):
```bash
lib/external/cmake/bin/cmake -G "Unix Makefiles" -DCMAKE_C_COMPILER=lib/external/gcc/bin/gcc -DCMAKE_CXX_COMPILER=lib/external/gcc/bin/g++ -S . -B build
lib/external/cmake/bin/cmake --build build
./build/ffte
```
Or run tests via CTest:
```bash
lib/external/cmake/bin/ctest --test-dir build
```

## Build and Test (VScode IDE)
I used VScode as IDE and corresponding configs are under `.vscode` folder. Again: if you want to change IDE, it is your responsibility too.

CMake kits for Windows and Linux are defined in `.vscode/cmake-kits.json`. To select a kit in VSCode: open the Command Palette (`Ctrl+Shift+P`) → **CMake: Select a Kit** → choose `w64devkit-x64-2.0.0 (Windows)` or `ARM GNU Toolchain 14.2.rel1 x86-64 (Linux)` depending on your platform.
