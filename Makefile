
CURDIR:= $(shell pwd)
TOOLCHAIN:= $(PREFIX)/bin/$(TARGET)
GAS:=$(TOOLCHAIN)-as
LD:=$(TOOLCHAIN)-ld
CC:=$(TOOLCHAIN)-gcc
INCLUDE:= -I./src/boot/include/ -I./src/include
CFLAGS:= -g -ffreestanding -falign-jumps -falign-functions -falign-labels -falign-loops -fstrength-reduce -fomit-frame-pointer -finline-functions -Wno-unused-function -fno-builtin -Werror -Wno-unused-label -Wno-cpp -Wno-unused-parameter -nostdlib -nostartfiles -nodefaultlibs -Iinc
BOOT_OBJ:= ./src/boot/boot.o
BOOT_BIN:= ./bin/boot.bin
KERNEL_OBJ:= ./build/kernel.asm.o
IDT_OBJ:= ./build/idt.asm.o
FILES:= ./build/kernel_init.o 
KERNEL_BIN:= ./bin/kernel.bin
LD_SCRIPT := ./src/linker.ld

all: $(BOOT_BIN) $(KERNEL_BIN)
	rm -rf ./bin/os.img
	dd if=$(BOOT_BIN) >> ./bin/os.img
	dd if=$(KERNEL_BIN) >> ./bin/os.img
	dd if=/dev/zero bs=512 count=100 >> ./bin/os.img
	

$(KERNEL_BIN): $(KERNEL_OBJ) $(FILES)
	$(LD) -g -relocatable  $^ -o ./build/kernelfull.o
	$(CC)  $(CFLAGS) -T$(LD_SCRIPT) -o $@ -ffreestanding -nostdlib ./build/kernelfull.o

./build/kernel_init.o: ./src/kernel/kernel_init.c
	$(CC) $(INCLUDE) $(CFLAGS) -std=gnu99 -c $< -o $@

$(KERNEL_OBJ): ./src/boot/kernel.S
	$(GAS) -g $< -o $@

$(BOOT_OBJ): ./src/boot/boot.S
	$(GAS) $< -o $@

$(BOOT_BIN): $(BOOT_OBJ)
	$(LD) -Ttext 0x7c00 --oformat=binary $< -o $@


clean:
	rm ./build/*.o
	rm ./bin/*.img ./bin/*.bin
