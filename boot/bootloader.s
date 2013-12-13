[BITS 16]
[ORG 0]

; сколько цилиндров считывать
%define CYLS_TO_READ 10

; максимальное количество ошибок чтения
; (повторять операцию чтения необходимо как минимум три раза,
; т.к. ошибки возможны из-за того, что мотор привода не разогнался)
%define MAX_READ_ERRORS 5

; Точка входа:
entry:

	cli ;на время установки стека запретим прерывания
	mov ax, cs
	mov ds, ax
	mov es, ax
	mov ss, ax
	mov sp, start

	sti ;разрешим прерывания

	;; Необходимо скопировать загрузчик в
	;; верхнюю часть доступной памяти, т.к.
	;; текущий код (находящийся по адресу 0x0:0x7c00
        ;; будет переписан загруженными с дискеты данными

	; В DS - адрес исходного сегмента
	mov ax, 0x07c0
	mov ds, ax

        ; В ES - адрес целевого сегмента
	mov ax, 0x9000
	mov es, ax

	; Копируем с 0
	xor si, si
	xor di, di

	; Копируем 128 двойных слов (512 байт)
	mov cx, 128
	rep movsd

	; Прыжок туда, где теперь находится бутсектор
	; (0x9000: 0)
	jmp 0x9000:start


	; следующий код выполняется по адресу 0x9000:0
start:

        ; занесем новые значения в сегментные регистры
	mov ax, cs
	mov ds, ax
	mov ss, ax

pusha
	; обрадуем пользователя сообщением
	mov si, msg_loading
	call kputs
popa


	; Дальнейшая процедура выполняет чтение цилиндра
	; начиная с указанного в DI плюс нулевой цилиндр (в самом конце)
	; В AX - адрес сегмента в который будут записаны считанные данные

	mov di, 1
	mov ax, 0x290
	xor bx, bx


.loop:
	mov cx, 0x50
	mov es, cx

	push di

	; Вычисляем какую головку использовать
	shr di, 1
	setc dh
	mov cx, di
	xchg cl, ch

	pop di

	; Уже все цилиндры считали?
	cmp di, CYLS_TO_READ
	je .quit

	call kread_cylinder

	;; Цилиндр считан по адресу 0x50:0x0 - 0x50:0x2400
	;; (линейный 0x500 - 0x2900)

	;; Выполним копирование этого блока по нужному адресу
	pusha
	push ds

	mov cx, 0x50
	mov ds, cx
	mov es, ax
	xor di, di
	xor si, si
	mov cx, 0x2400
	rep movsb

	pop ds
	popa

	; Увеличим DI, AX и повторим все сначала
	inc di
	add ax, 0x240
	jmp short .loop
.quit:

	; Мы считывали начиная с 1-го цилиндра!
	; (т.к. участок 0x50:0 использовался как буфер данных)
	; теперь он свободен и мы можем считать нулевой цилиндр в него

	mov ax, 0x50
	mov es, ax
	mov bx, 0
	mov ch, 0
	mov dh, 0
	call kread_cylinder

	; Прыжок на загруженный код
	jmp 0x0000:0x0700

kread_cylinder:
        ;; процедура читает заданный цилиндр
	;; ES:BX - буфер
	;; CH - цилиндр
	;; DH - головка

	; Сбросим счетчик ошибок
	mov [.errors_counter], byte 0

	pusha

	; Сообщим пользователю, какой цилиндр и головку читаем
	mov si, msg_cylinder
	call kputs
	mov ah, ch
	call kputhex
	mov si, msg_head
	call kputs
	mov ah, dh
	call kputhex
	mov si, msg_crlf
	call kputs

	popa
	pusha

.start:
	mov ah, 0x02
	mov al, 18
	mov cl, 1

	; Прерывание дискового сервиса BIOS
	int 0x13
	jc .read_error


	popa
	ret

.errors_counter:	db 0
.read_error:
        ; Если произошла ошибка, то увеличим счетчик,
	; и выведем сообщение с кодом ошибки

	inc byte [.errors_counter]
	mov si, msg_reading_error
	call kputs
	call kputhex
	mov si, msg_crlf
	call kputs

	; Счетчик ошибок превысил максимальное значение?
	cmp byte [.errors_counter], MAX_READ_ERRORS
	jl .start

	; Ничего не получилось :(
	mov si, msg_giving_up
	call kputs
	hlt
	jmp short $




	hex_table:	db "0123456789ABCDEF"
kputhex:
        ; Процедура преобразует число в ASCII-код
	; его шестнадцатеричного представления и выводит его
	; (Да, я знаю, что это можно сделать четырьмя командами :))

	pusha
	xor bx, bx
	mov bl, ah
	and bl, 11110000b
	shr bl, 4
	mov al, [hex_table+bx]
	call kputchar

	mov bl, ah
	and bl, 00001111b
	mov al, [hex_table+bx]
	call kputchar

	popa
	ret

	; Процедура выводит символ в AL на экран
kputchar:
	pusha
	mov ah, 0x0E
	int 0x10
	popa
	ret

	; Процедура выводит строку, на которую указывает SI, на экран
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


	; Служебные сообщения
msg_loading:	db "the Operating System is loading...", 0x0A, 0x0D, 0
msg_cylinder:	db "Cylinder:", 0
msg_head:	db ", head:",0
msg_reading_error:	db "Error reading from floppy. Errcode:",0
msg_giving_up:	db "Too many errors, giving up",0x0A,0x0D, "Reboot your system, please", 0
msg_crlf:	db 0x0A, 0x0D,0

	; Сигнатура бутсектора:
TIMES 510 - ($-$$) db 0
db 0x55, 0xAA
incbin 'second_loader.bin'
