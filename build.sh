#! /bin/bash

export PREFIX="$HOME/opt/cross"
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"

make PREFIX=${PREFIX} TARGET=${TARGET} all

qemu-system-x86_64 -D ./log.txt -d int -hda ./bin/os.img 
