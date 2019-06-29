#!/bin/sh

build()
{
    nasm -f bin -o $1.bin $1.asm
	qemu-system-i386 -fda $1.bin -boot a
}

build map
