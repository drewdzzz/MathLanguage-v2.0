;==========INCLUDE DREW LIB==========

segment .data
BUFFER_SIZE equ 100
buffer times BUFFER_SIZE db 0

segment .text

;==============================================================
;Entry:		    requires buffer and const BUFFER_SIZE
;				R8D	- int value
;Exit: 		    RDX - length of buffer
;				RSI - ptr to number in buffer
;				ES  = DS
;
;Destr:		    RCX RAX	R8D	R10B R9B
;==============================================================
	dec_format:
	mov rsi, buffer
	add rsi, BUFFER_SIZE
	xor r10b, r10b
	mov r9d, 10
	mov rcx, 9
	mov eax, r8d

	.dec_format_loop:
	dec rsi
	inc r10b

	xor edx, edx
	div r9d

	add edx, '0'
	mov [rsi], dl 
	LOOP .dec_format_loop

	xor rdx, rdx
	mov dl, r10b

	mov eax, ds
	mov es, eax
	mov al, '0'
	mov rcx, rdx
	dec rcx
	.skipping:
	cmp al, [rsi]
	jnz .break
	inc rsi
	dec rdx
	loop .skipping
	.break:
	ret


PRINT:
	call dec_format
	xor rax, rax
	inc rax
	xor rdi, rdi
	syscall
	ret


;==============================================================================
; In:  rsi - buf to input
; Out: rdx - string len
; Destr: rdi, rax
;==============================================================================
gets:
	push rsi

	mov rdi, 0
	mov rdx, 1

.continue:
	mov rax, 0
	syscall

	cmp byte [rsi], 10d
	je .exit

	inc rsi
	jmp .continue

.exit:
	mov rdx, rsi
	pop rsi
	sub rdx, rsi

	ret

toInt:
	xor r8, r8
	mov rdi, rsi
	add rdi, rdx
	mov rcx, 1
	mov rbx, 10

.loop:
	dec rdi
	mov bl, [rdi]
	sub bl, '0'
	mov rax, rcx
	mul bl
	add r8, rax
	mov rax, rcx
	mul rbx
	mov rcx, rax
	cmp rdi, rsi
	jne .loop
	mov rax, r8
	ret


SCAN:
	mov rsi, buffer
	call gets
	mov rsi, buffer
	call toInt
	ret
;==========INCLUDE DREW LIB==========


global _start
_start:
	push 0
	push 0
	call MAIN
	mov rax, 0x3C
	xor rdi, rdi
	syscall

FUNCTION0:
	push rbp
	mov rbp, rsp
	push qword [rbp + 16 + 8*0]
	push 1

	pop r10								;Conditional operator - a == b
	pop r11
	cmp r10, r11
	je COND_T2
	push 0
	jmp COND_F2
COND_T2:
	push 1
COND_F2:

	pop r10
	xor r11, r11
	cmp r10, r11
	je COND_F1
	push qword [rbp + 16 + 8*0]
	pop rax
	pop rbp
	ret
COND_F1:
	push qword [rbp + 16 + 8*0]
	push qword [rbp + 16 + 8*0]
	push 1
	pop r10
	pop r11
	sub r11, r10
	push r11

	call FUNCTION0
	pop r8
	push rax
	pop rax
	pop rdx
	mul rdx
	push rax
	pop rax
	pop rbp
	ret
	pop rbp
	ret

MAIN:
	push rbp
	mov rbp, rsp
	call SCAN								; function INPUT
	mov qword [rbp + 16 + 8*0], rax
push qword [rbp + 16 + 8*0]

	call FUNCTION0
	pop r8
	push rax
	pop qword [rbp + 16 + 8*0]
push qword [rbp + 16 + 8*0]
	
pop r8
								; PRINT function
	call PRINT


	pop rbp
ret
