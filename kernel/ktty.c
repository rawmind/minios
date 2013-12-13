#include "stdlib.h"
#include "stdarg.h"

#define VIDEO_WIDTH 80
#define VIDEO_HEIGHT 25
#define VIDEO_RAM 0xb8000

int cursor_position;
int cursor_attribute;
const char digits[] = "0123456789ABCDEF";
char num_buffer[65];



void init_tty() 
{
  cursor_position = 0;
  cursor_attribute = 7;
}


void textcolor(char c)
{
  cursor_attribute = c;
}


void clear()
{
  char *video = (char*)VIDEO_RAM;
  int i;

  for (i = 0; i < VIDEO_HEIGHT*VIDEO_WIDTH; i++) {
    *(video + i*2) = ' ';
  }
  
  cursor_position = 0;
}

void putchar(char c)
{
  char *video = (char*)VIDEO_RAM;
  int i;

  switch (c) {
  case '\n':
    cursor_position+=VIDEO_WIDTH;
    cursor_position-=cursor_position%VIDEO_WIDTH;
    break;
  case '\r':
	cursor_position-=(cursor_position%VIDEO_WIDTH);
	break;
  case '\t':
	  cursor_position+=4;
	  break;
  default:
    *(video + cursor_position*2) = c;
    *(video + cursor_position*2+1) = cursor_attribute;
    cursor_position++;
    break;
  }


  if(cursor_position>VIDEO_WIDTH*VIDEO_HEIGHT){
    for(i=VIDEO_WIDTH*2;i<=VIDEO_WIDTH*VIDEO_HEIGHT*2+VIDEO_WIDTH*2;i++){
      *(video+i-VIDEO_WIDTH*2)=*(video+i);
    }
    cursor_position-=VIDEO_WIDTH;
  }
}


void puts(const char *s)
{
  while(*s) {
    putchar(*s);
    s++;
  }

}

char *int_to_str(size_t value, unsigned char base) {
	size_t i = sizeof(num_buffer) - 1;
	num_buffer[i--] = '\0';
	do {
		num_buffer[i--] = digits[value % base];
		value = value / base;
	} while (value);

	return &num_buffer[i + 1];
}


void printf(char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	while (*fmt) {
		if (*fmt == '%') {
			fmt++;
			size_t arg = va_arg(args, size_t);
			switch (*fmt) {
				case '%':
					putchar('%');
					break;
				case 'c':
					putchar(arg);
					break;
				case 's':
					puts((char*)arg);
					break;
				case 'b':
					puts(int_to_str(arg, 2));
					break;
				case 'o':
					puts(int_to_str(arg, 8));
					break;
				case 'd':
					puts(int_to_str(arg, 10));
					break;
				case 'x':
					puts(int_to_str(arg, 16));
					break;
			}
		} else {
			putchar(*fmt);
		}
		fmt++;
	}
	va_end(args);
}

