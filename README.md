# ffte
Fast Fourier Transform for Embedded Systems in C and CPP. CPP has also Dynamic Fixed Point Number support (DynFix).
 * Power of 2 FFT's calculated by Cooley-Tukey
 * Non-power of 2 FFT's calculated by Bluestein
 * 2D FFT supported

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

## IDE
I used VScode as IDE and corresponding configs are under `.vscode` folder. Again: if you want to change IDE, it is your responsibility too.
