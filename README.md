# TinyLlama BIOS

This repo contains the tools necessary to build the [TinyLlama](https://github.com/eivindbohler/tinyllama) BIOS (Coreboot/SeaBIOS). 
Based on the 86Duino project from https://github.com/roboard/build-coreboot

Tested on Ubuntu 16.04 i386 and macOS Big Sur.

### Prerequisites
#### Ubuntu
Install various tools:
```
$ sudo apt install git build-essential m4 bison flex python texinfo gnat
```
#### macOS
1. Install [Homebrew](https://brew.sh)
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

If building is successful, the output ROM file is in the `out/` directory.
Another file is also made - `out/xxx-padded.rom` - that is padded with `0xFF` to fit 8 MB, for flashing directly to the SPI ROM with a hardware programmer. The coreboot/seabios ROM is located at the end of this 8 MB file.

