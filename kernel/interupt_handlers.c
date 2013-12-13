#include "code.h"
#include "rtc.h"


#define IRQ_HANDLER(func) void func (void);\
asm(#func ": pusha \n call _" #func " \n movb $0x20, %al \n outb $0x20 \n popa \n iret \n");\
void _ ## func(void)

#define IRQ_ERROR_HANDLER(func) void func (void);\
asm(#func ": pusha \n call _" #func " \n movb $0x20, %al \n outb $0x20 \n popa \n inc %esp \n inc %esp \n iretl \n");\
void _ ## func(void)



IRQ_HANDLER(irq_timer)
{
	RTC_Time rtcTime = get_time();
	printf("\rCurrent time: %x-%x-%x %x:%x:%x", rtcTime.year, rtcTime.month, rtcTime.day_of_month, rtcTime.hour, rtcTime.minute, rtcTime.second);
}

IRQ_HANDLER(irq_dumb) {
    puts("e\n");
    asm("hlt");
}

IRQ_HANDLER(irq_dumb0) {
    puts("e0\n");
    asm("hlt");
}

IRQ_HANDLER(irq_dumb1) {
    puts("e1\n");
    asm("hlt");
}

IRQ_HANDLER(irq_dumb2) {
    puts("e2\n");
    asm("hlt");
}

IRQ_HANDLER(irq_dumb3) {
    puts("e3\n");
    asm("hlt");
}

IRQ_HANDLER(irq_dumb4) {
    puts("e4\n");
    asm("hlt");
}

IRQ_HANDLER(irq_dumb5) {
    puts("e5\n");
    asm("hlt");
}

IRQ_HANDLER(irq_dumb6) {
    puts("e6\n");
    asm("hlt");
}

IRQ_HANDLER(irq_dumb7) {
    puts("e7\n");
    asm("hlt");
}

IRQ_ERROR_HANDLER(irq_dumb_error) {
    puts("e\n");
}

IRQ_ERROR_HANDLER(irq_dumb_error8) {
    puts("e8\n");
    asm("hlt");
}

IRQ_ERROR_HANDLER(irq_dumb_error9) {
    puts("e9\n");
    asm("hlt");
}

IRQ_ERROR_HANDLER(irq_dumb_error10) {
    puts("e10\n");
    asm("hlt");
}

IRQ_ERROR_HANDLER(irq_dumb_error11) {
    puts("e11\n");
    asm("hlt");
}

IRQ_ERROR_HANDLER(irq_dumb_error12) {
    puts("e12\n");
    asm("hlt");
}

IRQ_ERROR_HANDLER(irq_dumb_error13) {
    puts("e13\n");
    asm("hlt");
}

IRQ_ERROR_HANDLER(irq_dumb_error14) {
    puts("e14\n");
    asm("hlt");
}

IRQ_HANDLER(irq_dumb16) {
    puts("e16\n");
    asm("hlt");
}

IRQ_ERROR_HANDLER(irq_dumb_error17) {
    puts("e17\n");
    asm("hlt");
}


IRQ_HANDLER(irq_kb)
{
    static unsigned char shift = 0;
    unsigned char scancode, ascii;
    unsigned char creg;
    scancode = inb(0x60);
    switch(scancode) {


        case 0x36:
        case 0x2A:
            shift = 1;
            break;
        case 0x36 + 0x80:
        case 0x2A + 0x80:
            shift = 0;
            break;

        default:
            if(scancode >= 0x80) {

            } else {

                if(shift){
                    ascii = codes_sh[scancode];

                } else {
                    ascii = codes[scancode];
                }

                if(ascii != 0) {
                	putchar(ascii);
                }
            }
            break;
    }

    creg = inb(0x61);
    creg |= 0x80;
    outb(0x61, creg);

}

void init_interrupts()
{

 install_interupt_handle(0, &irq_dumb0, 0x8f);
 install_interupt_handle(1, &irq_dumb1, 0x8f);
 install_interupt_handle(2, &irq_dumb2, 0x8e);
 install_interupt_handle(3, &irq_dumb3, 0x8e);
 install_interupt_handle(4, &irq_dumb4, 0x8e);
 install_interupt_handle(5, &irq_dumb5, 0x8f);
 install_interupt_handle(6, &irq_dumb6, 0x8f);
 install_interupt_handle(7, &irq_dumb7, 0x8f);

 install_interupt_handle(8, &irq_dumb_error8, 0x8f);
 install_interupt_handle(9, &irq_dumb_error9, 0x8f);
 install_interupt_handle(10, &irq_dumb_error10, 0x8f);
 install_interupt_handle(11, &irq_dumb_error11, 0x8f);
 install_interupt_handle(12, &irq_dumb_error12, 0x8f);
 install_interupt_handle(13, &irq_dumb_error13, 0x8f);
 install_interupt_handle(14, &irq_dumb_error14, 0x8f);

 install_interupt_handle(16, &irq_dumb16, 0x8f);
 install_interupt_handle(17, &irq_dumb_error17, 0x8f);

 int i;
 for (i = 17; i < 32; ++i) {
     install_interupt_handle(i, &irq_dumb_error, 0x8f);
 }

 install_interupt_handle(0x20, &irq_timer, 0x8e);
 install_interupt_handle(0x21, &irq_kb, 0x8e);

 for (i = 0x22; i < 0x70; ++i) {
        install_interupt_handle(i, &irq_dumb, 0x8e);
    }


i_setup();
i_enable();
}

