#ifndef IOPORTS_H_
#define IOPORTS_H_

#include "stdlib.h"

__inline__ void outb(uint16 port, uint8 value);
__inline__ uint8 inb(uint16 port);
__inline__ void outw(uint16 port, uint16 value);
__inline__ uint16 inw(uint16 port);

#endif /* IOPORTS_H_ */
