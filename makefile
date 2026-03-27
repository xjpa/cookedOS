UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
CROSS ?= x86_64-elf
else
CROSS ?=
endif

ifeq ($(strip $(CROSS)),)
CC := gcc
LD := ld
OBJCOPY := objcopy
else
CC := $(CROSS)-gcc
LD := $(CROSS)-ld
OBJCOPY := $(CROSS)-objcopy
endif

all: bootloader

bootloader:
	nasm kernel/boot.asm -f bin -o kernel/bin/boot.bin
	nasm kernel/kernel_entry.asm -f elf -o kernel/bin/kernel_entry.bin
	
	$(CC) -m32 -ffreestanding -c kernel/master.c -o kernel/bin/kernel.o
	$(LD) -m elf_i386 -o kernel/bin/kernel.img -Ttext 0x1000 kernel/bin/kernel_entry.bin kernel/bin/kernel.o

	$(OBJCOPY) -O binary -j .text kernel/bin/kernel.img kernel/bin/kernel.bin
	cat kernel/bin/boot.bin kernel/bin/kernel.bin > os.img

clean:
	rm -f kernel/boot.img

run: all
	qemu-system-x86_64 -drive format=raw,file=os.img

run-vnc: all
	qemu-system-x86_64 -drive format=raw,file=os.img -display none -vnc :0
