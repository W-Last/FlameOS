#!/bin/bash
set -e

# Variables: adjust these as needed.
PREFIX=/usr/local/cross
TARGET=i686-elf
BINUTILS_VER=2.40
GCC_VER=13.2.0

# Make sure required packages are installed (on Debian/Ubuntu)
sudo apt-get update
sudo apt-get install -y build-essential bison flex libgmp-dev libmpc-dev libmpfr-dev texinfo wget

# Create a directory for the toolchain sources.
mkdir -p ~/src && cd ~/src

# Download Binutils
wget https://ftp.gnu.org/gnu/binutils/binutils-${BINUTILS_VER}.tar.gz
tar xf binutils-${BINUTILS_VER}.tar.gz

# Download GCC
wget https://ftp.gnu.org/gnu/gcc/gcc-${GCC_VER}/gcc-${GCC_VER}.tar.gz
tar xf gcc-${GCC_VER}.tar.gz

# Build Binutils
mkdir -p build-binutils && cd build-binutils
../binutils-${BINUTILS_VER}/configure --target=${TARGET} --prefix=${PREFIX} --with-sysroot --disable-nls --disable-werror
make -j$(nproc)
sudo make install
cd ..

# Build GCC (only C compiler)
mkdir -p build-gcc && cd build-gcc
../gcc-${GCC_VER}/configure --target=${TARGET} --prefix=${PREFIX} --disable-nls --enable-languages=c --without-headers
make -j$(nproc) all-gcc
sudo make install-gcc
cd ..

# Add the new cross-compiler to your PATH (this line can be added to your ~/.bashrc)
echo "export PATH=${PREFIX}/bin:\$PATH" >> ~/.bashrc
export PATH=${PREFIX}/bin:$PATH

# Verify the cross-compiler
i686-elf-gcc --version

echo "Cross-compiler built and installed successfully."
