OBJECTS=startup.o kernel.o ktty.o intslib.o ioports.o interupt_handlers.o memory_manager.o stdlib.o rtc.o 
#pci.o
CFLAGS= -Wall -ffreestanding -c -m32 -O2
LDFLAGS= -m elf_i386 --oformat binary -Ttext 0x200000


kernel.bin: $(OBJECTS)
	ld $(LDFLAGS) -o kernel.bin $(OBJECTS)

startup.o: startup.s
	nasm -felf -o startup.o startup.s

kernel.o: kernel.c
	gcc $(CFLAGS) -o kernel.o kernel.c

ktty.o: ktty.c
	gcc $(CFLAGS) -o ktty.o ktty.c

intslib.o: intslib.c
	gcc $(CFLAGS) -o intslib.o intslib.c

ioports.o: ioports.c
	gcc $(CFLAGS) -o ioports.o ioports.c

interupt_handlers.o: interupt_handlers.c rtc.h
	gcc $(CFLAGS) -o interupt_handlers.o interupt_handlers.c

memory_manager.o: memory_manager.c memory_manager.h
	gcc $(CFLAGS) -o memory_manager.o memory_manager.c	

stdlib.o: stdlib.c stdlib.h
	gcc $(CFLAGS) -o stdlib.o stdlib.c	

rtc.o: rtc.c rtc.h
	gcc $(CFLAGS) -o rtc.o rtc.c
	
#pci.o: pci.c pci.h
#	gcc $(CFLAGS) -o pci.o pci.c
			
clean: 
	rm *.o
	
