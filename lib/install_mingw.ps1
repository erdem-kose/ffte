cls

$cwd=Split-Path -Path $PSCommandPath 
$cwd=$cwd -replace '\\', '/'

$gcc_filename = "w64devkit-x64"
$gcc_version = "2.1.0"

$URL = "https://github.com/skeeto/w64devkit/releases/download/v$gcc_version/$gcc_filename-$gcc_version.exe"

$Path = "$cwd/external/"
$InstallerPath = "$Path/$gcc_filename-$gcc_version.exe"
$InstallDir = "$Path/$mingw"

# Create the directory if it doesn't exist
If (!(Test-Path -Path $Path)) {
    New-Item -ItemType Directory -Path $Path
}

# Download the installer
Invoke-WebRequest -URI $URL -OutFile $InstallerPath

# Run the installer
Start-Process -FilePath $InstallerPath -ArgumentList "/S", "/D=$InstallDir" -Wait

Rename-Item "$Path/w64devkit/" -NewName "mingw"

# Clean up installer after installation if needed
Remove-Item -Path $InstallerPath -Force

