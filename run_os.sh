#!/bin/sh
# Force build everything
make clean && make

# Boot up the raw compiled kernel binary file
qemu-system-i386 -kernel os-kernel.bin -nographic
