[BITS 32]
[EXTERN kernel_main]
[GLOBAL _start]
_start:
	movzx edx, dl
	push edx
	push esi
	push ebx
	lgdt [gdtr]
	mov esp, 0x200000-4
	call kernel_main
	jmp $

gdt:
	dq 0
	dq 0x00CF9A000000FFFF
	dq 0x00CF92000000FFFF
gdtr:
	dw $ - gdt
	dd gdt
