add-symbol-file ./build/kernelfull.o 0x100000
break kernel_init
break print
target remote | qemu-system-x86_64 -S -D ./log.txt -d int -gdb stdio -hda ./bin/os.img
