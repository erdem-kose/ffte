#!/bin/bash
# Copyright (c) [2022-2026] Erdem Kose

set -e

cwd="$(dirname "$(realpath "$0")")"

gcc_version="2024.05-1"
gcc_target="x86_64-buildroot-linux-gnu"
gcc_filename="x86-64--glibc--stable-${gcc_version}"

URL="https://toolchains.bootlin.com/downloads/releases/toolchains/x86-64/tarballs/${gcc_filename}.tar.xz"

path="$cwd/../external"
archive="$path/${gcc_filename}.tar.xz"
install_dir="$path/gcc"

# Create the directory if it doesn't exist
rm -rf "$install_dir"
mkdir -p "$path"

# Download the archive
curl -L -o "$archive" "$URL"

# Extract the archive
tar -xJf "$archive" -C "$path"

mv "$path/${gcc_filename}" "$install_dir"

# Create unprefixed symlinks so cmake finds gcc / g++ without the target prefix
cd "$install_dir/bin"
for tool in gcc g++ ar as ld nm objcopy objdump ranlib strip; do
    [ -f "${gcc_target}-${tool}" ] && ln -sf "${gcc_target}-${tool}" "${tool}"
done

# Clean up archive after installation
rm -f "$archive"

# Download ninja build tool
ninja_zip="$path/ninja-linux.zip"
curl -L -o "$ninja_zip" "https://github.com/ninja-build/ninja/releases/download/v1.12.1/ninja-linux.zip"
python3 -m zipfile -e "$ninja_zip" "$install_dir/bin/"
chmod +x "$install_dir/bin/ninja"
rm -f "$ninja_zip"
