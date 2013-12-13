[BITS 16]
[ORG 0x700]

	;; Загрузим в сегментные регистры 0 и установим стек

	cli
	mov ax, 0
	mov ds, ax
	mov es, ax
	mov ss, ax
	mov sp, 0x700
	sti

	;; Выведем приветствие на экран
	mov si, msg_intro
	call kputs

	;; Сообщение о том, что собираемся в PM
	mov si, msg_entering_pmode
	call kputs

	;; Отключим курсор, чтобы не мозолил глаза
	mov ah, 1
	mov ch, 0x20
	int 0x10

	;; Установим базовый вектор контроллера прерываний в 0x20
	mov al,00010001b
	out 0x20,al
	mov al,0x20
	out 0x21,al
	mov al,00000100b
	out 0x21,al
	mov al,00000001b
	out 0x21,al


	;; Отключим прерывания
	cli

	;; Загрузка регистра GDTR:
	lgdt [gd_reg]

	;; Включение A20:
	in al, 0x92
	or al, 2
	out 0x92, al

	;; Установка бита PE регистра CR0
	mov eax, cr0
	or al, 1
	mov cr0, eax

	;; С помощью длинного прыжка мы загружаем
	;; селектор нужного сегмента в регистр CS
	;; (напрямую это сделать нельзя)
	jmp 0x8: _protected


	;; Эта функция вывода строки работает
	;; в реальном режиме!
	;; (использует прерывание 0x10 BIOS)
kputs:
	pusha
.loop:
	lodsb
	test al, al
	jz .quit
	mov ah, 0x0e
	int 0x10
	jmp short .loop
.quit:
	popa
	ret


	;; Следующий код - 32-битный
[BITS 32]
	;; Сюда будет передано управление
	;; после входа в PM
_protected:
	;; Загрузим регистры DS и SS селектором
	;; сегмента данных
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov ss, ax

	;; Наше ядро (kernel.bin) слинковано по адресу 2мб
	;; Переместим его туда

	;; kernel_binary - метка, после которой
	;; вставлено ядро
	;; (фактически - его линейный адрес)
	mov esi, kernel_binary

	;; адрес, по которому копируем
	mov edi, 0x200000


	;; Размер ядра в двойных словах
	;; (65536 байт)
	mov ecx, 0x4000

	;; Поехали :)
	rep movsd

	;; Ядро скопировано, передаем управление ему
	jmp 0x200000


gdt:
	dw 0, 0, 0, 0	; Нулевой дескриптор

	db 0xFF		; Сегмент кода с DPL=0
	db 0xFF		; Базой=0 и Лимитом=4 Гб
	db 0x00
	db 0x00
	db 0x00
	db 10011010b
	db 0xCF
	db 0x00

	db 0xFF		; Сегмент данных с DPL=0
	db 0xFF		; Базой=0 и Лимитом=4Гб
	db 0x00
	db 0x00
	db 0x00
	db 10010010b
	db 0xCF
	db 0x00


	;; Значение, которое мы загрузим в GDTR:
gd_reg:
	dw 8192
	dd gdt


msg_intro:	db "Secondary bootloader received control", 0x0A, 0x0D, 0
msg_entering_pmode:	db "Entering protected mode...", 0x0A, 0x0D, 0


kernel_binary:
	incbin 'kernel.bin'
