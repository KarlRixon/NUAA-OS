%define CR0_PE		(1 << 0)
%define CR0_MP		(1 << 1)
%define CR0_EM		(1 << 2)
%define CR0_TS		(1 << 3)
%define CR0_WP		(1 << 16)
%define CR0_PG		(1 << 31)

        org 0x7c00
        [bits 16]
        ; First, BIOS loads the bootsector into 0000:7C00.
        cli
        xor     ax, ax
        mov     ds, ax
        mov     ss, ax
        mov     sp, 0

        ; Switch to protect mode 
        lgdt    [gdt_desc]
        mov     eax, cr0         
        or      eax, CR0_PE
        mov     cr0, eax
        jmp     0x08:start32

        [bits 32]
start32:
        ; In protect mode
        cli
        mov     ax, 0x10
        mov     ds, ax
        mov     es, ax
        mov     ss, ax
        mov     esp, 0x10000

		mov		edx, 0xB8000		;保护模式20位
		mov		ebx, msg
		mov		cx, msg_len			;字符串长度、循环次数
s:
        mov     al, [ebx]
        mov     ah, 0x0c
		inc		ebx
        mov     [edx], ax
		add		edx, 2
		loop	s
		
		mov		al, ' '
		mov		cx, (4000-msg_len)
space:	
		mov		[edx], ax
		add		edx, 2
		loop	space

        jmp     $

msg:	db	"Hello world in protected mode", 0
		msg_len	equ $-msg-1
		
        align   8                       
gdt:    dw      0,0,0,0         ; dummy

        dw      0xFFFF          ; limit=4GB
        dw      0x0000          ; base address=0
        dw      0x9A00          ; code read/exec
        dw      0x00CF          ; granularity=4096,386

        dw      0xFFFF          ; limit=4GB
        dw      0x0000          ; base address=0
        dw      0x9200          ; data read/write
        dw      0x00CF          ; granularity=4096,386

        align   8
gdt_desc: 
        dw      23              ; gdt limit=sizeof(gdt) - 1
        dw      gdt
		
        times   510-($-$$) db 0
        dw      0xAA55