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

# Install requirements (Windows)
To install required software, please follow (you may need to disable antivirus for these, your responsibility!)

```powershell
lib/install_mingw.ps1
lib/install_cmake.ps1
lib/install_googletest.ps1
lib/install_plplot.ps1
```

all of these will be local installations under this folder `lib/external`. So you don't need to worry if it messes with your local installations. All the examples are for Windows and MinGW. Conversion to other OS will be your responsibilitiy, also.

# Install requirements (Linux)
```bash 
sudo apt install libplplot-dev
sudo apt install printer-driver-cups-pdf
``` 

# IDE
I used VScode as IDE and corresponding configs are under `.vscode` folder. Again: if you want to change IDE, it is your responsibility too.
