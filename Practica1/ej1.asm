segment .data
_division_by_cero_s db "Division por cero"
_out_of_range_s db "Indice fuera de rango"
segment .bss
__esp resd 1
_b1 resd 1
_x resd 1
segment .text
global main
extern scan_int, print_int, scan_float, print_float, scan_boolean, print_boolean
extern print_endofline, print_blank, print_string
extern alfa_malloc, alfa_free, ld_float
; -----------------------
; PROCEDIMIENTO PRINCIPAL
; -----------------------
main:
mov [__esp], esp
push dword 1
pop eax
mov [_b1], eax
while_1:
push dword _b1
pop dword eax
mov eax, [eax]
cmp eax, 0
je whilend_1
push dword _x
call scan_int
add esp, 4
push dword _x
push dword 4
pop dword ebx
pop dword eax
mov eax, [eax]
cmp eax, ebx
jg more_2
push dword 0
jmp more_end_2
more_2:
push dword 1
more_end_2:
pop eax
mov [_b1], eax
push dword _b1
pop dword eax
mov eax, [eax]
cmp eax, 0
je else_3
push dword _x
pop dword eax
mov eax, [eax]
push dword eax
call print_int
add esp, 4
call print_endofline
jmp endif_3
else_3:
push dword 0
pop dword eax
push dword eax
call print_boolean
add esp, 4
call print_endofline
endif_3:
jmp while_1
whilend_1:
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
