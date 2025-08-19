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

	// __asm__ volatile (
    // "mov $5, %eax\n\t"
    // "xor %edx, %edx\n\t"
    // "mov $0, %ecx\n\t"   // divisor = 0
    // "div %ecx\n\t"       // divide eax by ecx (0) → causes #DE
	// ); 	// Test that division by 0 is being handled

	// printf("After dividing by zero (this should not print)\n");
}
