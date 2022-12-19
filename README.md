# TinyLlama BIOS

This repo contains the tools necessary to build the [TinyLlama](https://github.com/eivindbohler/tinyllama) BIOS (Coreboot/SeaBIOS).  
Based on the 86Duino project from https://github.com/roboard/build-coreboot

Tested on:
* Ubuntu 16.04 i386
* macOS Big Sur x86_64
* macOS Monterey x86_64 and arm64

Other operating systems and architectures are probably easy to get working as well, but your mileage may vary.

### Prerequisites
#### Ubuntu
Install various tools:
```
$ sudo apt install git build-essential m4 bison flex python texinfo gnat
```
#### macOS (x86_64)
For Apple Silicon (arm64) CPUs, [look here](prerequisites_arm.md).
1. Install [Homebrew](https://brew.sh):
```
$ /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```
2. Install gcc version 6:
```
$ brew install gcc@6
$ ln -s /usr/local/bin/gcc-6 /usr/local/bin/gcc
$ ln -s /usr/local/bin/g++-6 /usr/local/bin/g++
```
3. Install gnu-tar and wget:
```
$ brew install gnu-tar wget
```
### Build cross compiler
```
$ ./build-xgcc.sh
```

### Build the ROM file
```
$ make
```

If building is successful, the 1 MB output ROM file will be in the `out/` directory.
Another file is also made - `out/xxx-padded.rom` - padded with `0xFF` to fit 8 MB, for flashing directly to the SPI ROM using a hardware programmer. The Coreboot/SeaBIOS ROM will be located at the end of this 8 MB file.

