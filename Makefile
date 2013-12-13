all: image

image:
	make -C kernel
	make -C boot
	
assemble:
	dd if=/dev/zero of=disk.img bs=1024 count=1440 && dd if=boot/bootloader.img of=disk.img conv=notrunc
		
clean:
	make -C kernel clean
	make -C boot clean
	rm disk.img	
	
run: image assemble
	qemu -fda disk.img -hda fs.img -boot a 
	
