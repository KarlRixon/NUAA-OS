%define KERNEL_START             0xC0000000

        extern main
        global _start

        section .text 
_start:
        cli
        mov     esp, KERNEL_START + 0x10000

        ; Jump to C
        jmp     main