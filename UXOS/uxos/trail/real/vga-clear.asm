        org     0x7c00
        [bits 16]
        mov     ax, 0xB800     
        mov     es, ax
		mov		bp, 0x00
		mov		cx, 2000
        mov     ah, 0x00
        mov     al, ' '
loop_clear:
        mov     [es:bp], ax
		inc		bp
		inc		bp
		loop 	loop_clear
stop:   jmp     stop

        times 510-($-$$) db 0
        dw      0xAA55
