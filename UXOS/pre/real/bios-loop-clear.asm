        org     0x7c00
        [bits 16]
        mov     ax, 0xB800     
        mov     es, ax
		mov		bp, msg
		mov     ax, 0x1300
		mov		bx, 0x0000
		mov 	cx, 50	;50行
		mov		dh, 0	;第0行开始
loop_line:
		push	cx
		mov		cx, 80	;80列
		mov		dl, 0	;第0列开始
loop_row:
		push	cx
		mov		cx, 0x01
		int		0x10
		inc		dl		;列值加1
		pop		cx
		loop	loop_row
		pop		cx
		inc		dh		;行值加1
		loop	loop_line
stop:   jmp     stop
msg:	db		" ", 0
        times 510-($-$$) db 0
        dw      0xAA55
