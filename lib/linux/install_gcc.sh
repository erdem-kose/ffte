#!/bin/bash
# Copyright (c) [2022-2026] Erdem Kose

set -e

cwd="$(dirname "$(realpath "$0")")"

gcc_version="14.2.rel1"
gcc_host="x86_64"
gcc_target="x86_64-none-linux-gnu"
gcc_filename="arm-gnu-toolchain-${gcc_version}-${gcc_host}-${gcc_target}"

URL="https://developer.arm.com/-/media/Files/downloads/gnu/${gcc_version}/binrel/${gcc_filename}.tar.xz"

path="$cwd/external"
archive="$path/${gcc_filename}.tar.xz"
install_dir="$path/gcc"

# Create the directory if it doesn't exist
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
