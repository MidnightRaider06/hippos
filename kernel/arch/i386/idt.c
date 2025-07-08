#include <kernel/idt.h>
#include <stdint.h>
#include <stdio.h>

__attribute__((aligned(0x10))) 
static idt_entry_t idt[256]; // Create an array of IDT entries; aligned for performance

static idtr_t idtr;

// General exception handler
void exception_handler() {
    __asm__ volatile ("cli; hlt"); // Completely hangs the computer
}