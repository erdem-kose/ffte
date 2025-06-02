cls

$cwd=Split-Path -Path $PSCommandPath 
$cwd=$cwd -replace '\\', '/'

$cmake_version = "3.31.5"

$URL = "https://github.com/Kitware/CMake/releases/download/v$cmake_version/cmake-$cmake_version-windows-x86_64.zip"

$Path = "$cwd/external/"
$InstallerPath = "$Path/cmake-$cmake_version-windows-x86_64.zip"
$InstallDir = "$Path/"

# Download the installer
Invoke-WebRequest -URI $URL -OutFile $InstallerPath

# Run the installer
Expand-Archive "$InstallerPath" -DestinationPath "$InstallDir"
Rename-Item "$InstallDir/cmake-$cmake_version-windows-x86_64" -NewName "cmake"

# Clean up installer after installation if needed
Remove-Item -Path $InstallerPath -Force

