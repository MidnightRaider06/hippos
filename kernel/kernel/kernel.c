#include <stdio.h>

#include <kernel/tty.h>
#include <kernel/gdt.h>
#include <kernel/idt.h>

void kernel_main(void) {
	terminal_initialize();
	printf("Hello, kernel World!\n");
	
	initgdt();
	printf("GDT successful\n");

	initidt();
	printf("IDT successful\n");

}
