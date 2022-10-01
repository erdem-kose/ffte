# ffte
Fast Fourier Transform for Embedded Systems in C. 
 * Power of 2 FFT's calculated by Cooley-Tukey
 * Non-power of 2 FFT's calculated by Bluestein
 * 2D FFT supported

 * Demo script `main.c` requires 
    ```bash 
    sudo apt install libplplot-dev
    sudo apt install printer-driver-cups-pdf
    ``` 
 * For AVX2 support, add compiler options `-DFFTE_AVX_ENABLE -mavx2 -pthread`, or CMake option `-DFFTE_AVX_ENABLE=ON`
