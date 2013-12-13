CFLAGS=-ffreestanding -c -m32

bootloader: bootloader.s second_loader.bin
	 nasm -fbin -o bootloader.img bootloader.s
	 
second_loader.bin: second_loader.s kernel.bin
	nasm -fbin -o second_loader.bin second_loader.s

kernel.bin:
	cp ../kernel/kernel.bin .
	
clean:
	rm second_loader.bin
	rm bootloader.img
	rm kernel.bin
		