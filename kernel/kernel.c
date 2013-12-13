#include "memory_manager.h"
char coddes[] = {0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 8,'\t','q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']','\n', 0,'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',0,'\\', 'z', 'x', 'c', 'v','b', 'n', 'm', ',', '.', '/',0,'*',0,' ',0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,'-', 0, 0, 0,'+', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

typedef struct {
	long a0;
	long a1;
	long a2;
	long a3;
	long a4;
	long a5;
	long a6;
	long a7;
} s32b;

typedef struct {
	unsigned long long base;
	unsigned long long size;
} BootModuleInfo;


void kernel_main(char boot_disk_id, void *memory_map, BootModuleInfo *boot_module_list)
{
	init_memory_manager(memory_map);
	init_tty();
	clear();
	init_interrupts();

	int arr[10000];
	textcolor(2);
	printf("Structure size: %d\n",sizeof(arr));
	printf("kernel_page_dir = 0x%x\n", kernel_page_dir);
	printf("memory_size = %d MB\n", memory_size / 1024 / 1024);
	printf("get_page_info(kernel_page_dir, 0xB8000) = 0x%x\n", get_page_info(kernel_page_dir, (void*)0xB8000));

} 


