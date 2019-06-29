#!/bin/sh
set -e
nasm -f bin -o boot/boot.img boot/boot.asm

nasm -f elf -o cpu/entry.o cpu/entry.asm
gcc -c -m32 -fno-stack-protector -nostdinc -I. -o kernel/main.o kernel/main.c
gcc -c -m32 -fno-stack-protector -nostdinc -I. -o dev/vga.o dev/vga.c
gcc -c -m32 -fno-stack-protector -nostdinc -I. -o utils/stdio.o utils/stdio.c

ld -m elf_i386 -Ttext=0xC0010000 -o boot/uxos.elf cpu/entry.o kernel/main.o dev/vga.o utils/stdio.o
objcopy -O binary boot/uxos.elf boot/uxos.img

cat boot/boot.img boot/uxos.img >boot/fd.img
