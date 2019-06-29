CC     := gcc
CFLAGS := -c -m32 -fno-stack-protector -nostdinc -I.
OBJS   := cpu/entry.o kernel/main.o dev/vga.o utils/stdio.o

fd.img: boot.img uxos.img
	@echo "Generate fd.img" 
	@cat boot/boot.img boot/uxos.img >boot/fd.img

boot.img:
	nasm -f bin -o boot/boot.img boot/boot.asm

uxos.img: $(OBJS) 
	@echo "LD -o $@" 
	@ld -m elf_i386 -Ttext=0xC0010000 -o boot/uxos.elf $(OBJS)
	@objcopy -O binary boot/uxos.elf boot/uxos.img

cpu/entry.o: cpu/entry.asm
	nasm -f elf -o cpu/entry.o cpu/entry.asm

%.o: %.c
	@echo "CC -o $@ $<" 
	@$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f boot/fd.img boot/boot.img boot/uxos.img */*.o
