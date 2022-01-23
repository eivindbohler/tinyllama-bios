#! /usr/bin/env python
# Write 240 byte crossbar data into ROM file address {size-0x300}.
import sys
import os
def main():
    print("write-crossbar.py : Write CrossBar data into ROM file")
    rom_fn, cb_fn = sys.argv[1], sys.argv[2]
    print("ROM file : %s, CrossBar file = %s" % (rom_fn, cb_fn))
    cb_data = open(cb_fn, "rb").read()
    if len(cb_data) != 240:
        print("CrossBar data file length error")
        sys.exit(1)
    fs = os.stat(rom_fn).st_size
    if fs not in [256 * 1024, 512 * 1024, 1024 * 1024, 2048 * 1024, 4096 * 1024]:
        print("ROM file file length error")
        sys.exit(1)
    rom_f = open(rom_fn, "r+b")
    rom_f.seek(-0x300, 2)  # seek from the end of file
    rom_f.write(cb_data)
    print("Write CrossBar ok")

if __name__ == "__main__":
    main()
