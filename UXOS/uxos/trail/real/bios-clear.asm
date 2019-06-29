		org		0x7c00
		[bits 16]
		mov		ah, 0x06
		mov		al, 0x00
		mov		ch, 0x00
		mov		cl, 0x00
		mov		dh, 24
		mov		dl, 79
		mov		bh, 0x07
		int 	0x10
stop:	jmp		stop
		times	510-($-$$) db 0
		dw		0xAA55