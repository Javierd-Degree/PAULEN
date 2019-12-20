;D:	main
;D:	{
;D:	int
;D:	x
;D:	,
;D:	y
;D:	;
;D:	scanf
segment .bss
__esp resd 1
__x resd 1
__y resd 1
segment .text
global main
extern malloc, free
extern scan_int, print_int, scan_boolean, print_boolean
extern print_endofline, print_blank, print_string
; -----------------------
; PROCEDIMIENTO PRINCIPAL
; -----------------------
main:
mov [__esp], esp
;D:	x
push dword _x
call scan_int
add esp, 4
;D:	;
;D:	y
;D:	=
;D:	1
push dword 1
;D:	;
pop eax
mov [_y], eax
;D:	while
while_0:
;D:	(
;D:	(
;D:	x
;D:	>
push dword _x
;D:	1
push dword 1
;D:	)
pop dword ebx
pop dword eax
mov eax, [eax]
cmp eax, ebx
jg more_1
push dword 0
jmp more_end_1
more_1:
push dword 1
more_end_1:
;D:	)
pop dword eax
cmp eax, 0
je whilend_0
;D:	{
;D:	y
;D:	=
;D:	x
;D:	*
push dword _x
;D:	y
;D:	;
push dword _y
pop dword ebx
mov ebx, [ebx]
pop dword eax
mov eax, [eax]
imul ebx
push dword eax
pop eax
mov [_y], eax
;D:	x
;D:	=
;D:	x
;D:	-
push dword _x
;D:	1
push dword 1
;D:	;
pop dword ebx
pop dword eax
mov eax, [eax]
sub eax, ebx
push dword eax
pop eax
mov [_x], eax
;D:	}
jmp while_0
whilend_0:
;D:	printf
;D:	y
;D:	;
push dword _y
pop dword eax
mov eax, [eax]
push dword eax
call print_int
add esp, 4
call print_endofline
;D:	}
_end:
mov dword esp, [__esp]
ret
_division_by_cero:
push dword _division_by_cero_s
call print_string
add esp, 4
jmp _end
_out_of_range:
push dword _out_of_range_s
call print_string
add esp, 4
jmp _end
