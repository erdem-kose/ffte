#!/bin/bash
# Copyright (c) [2022-2026] Erdem Kose

set -e

cwd="$(dirname "$(realpath "$0")")"

gcc_version="2024.05-1"
gcc_target="x86_64-buildroot-linux-gnu"
gcc_filename="x86-64--glibc--stable-${gcc_version}"

URL="https://toolchains.bootlin.com/downloads/releases/toolchains/x86-64/tarballs/${gcc_filename}.tar.bz2"

path="$cwd/../external"
archive="$path/${gcc_filename}.tar.bz2"
install_dir="$path/gcc"

# Create the directory if it doesn't exist
mkdir -p "$path"

# Download the archive
curl -L -o "$archive" "$URL"

# Extract the archive
tar -xjf "$archive" -C "$path"

mv "$path/${gcc_filename}" "$install_dir"

# Create unprefixed symlinks so cmake finds gcc / g++ without the target prefix
cd "$install_dir/bin"
for tool in gcc g++ ar as ld nm objcopy objdump ranlib strip; do
    [ -f "${gcc_target}-${tool}" ] && ln -sf "${gcc_target}-${tool}" "${tool}"
done

# Clean up archive after installation
rm -f "$archive"
