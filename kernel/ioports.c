#include "ioports.h"

__inline__ void outb(uint16 port, uint8 value)
{
	asm volatile ("outb %1, %0"::"dN"(port),"a"(value));
}

__inline__ uint8 inb(uint16 port)
{
    uint8 value;
    asm volatile ("inb %1, %0":"=a"(value):"dN"(port));
    return value;
}

__inline__ void outw(uint16 port, uint16 value)
{
	asm volatile ("inw %1, %0":"=a"(value):"dN"(port));
}

__inline__ uint16 inw(uint16 port)
{
	uint16 value;
	asm volatile ("inw %1, %0":"=a"(value):"dN"(port));
	return value;
}
