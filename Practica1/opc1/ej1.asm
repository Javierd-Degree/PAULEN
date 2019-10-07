segment .data
segment .bss
__esp resd 1
_x resd 1
_y resd 1
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
push dword _x
call scan_int
add esp, 4
push dword _y
call scan_int
add esp, 4
mov dword ecx, [_x]
mov dword ebx, [_y]
while:
cmp ebx, 0
jz fin
add ecx, ebx
push dword ecx
pop dword eax
push dword eax
call print_int
add esp, 4
call print_endofline
push dword _y
call scan_int
add esp, 4
mov dword ebx, [_y]
jmp while
fin:
mov dword esp, [__esp]
ret
