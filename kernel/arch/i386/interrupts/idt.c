#include <kernel/idt.h>
#include <kernel/pic8259.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

__attribute__((aligned(0x10))) 
static idt_entry_t idt[256]; // Create an array of IDT entries; aligned for performance

#define IDT_MAX_DESCRIPTORS 32
static idtr_t idtr;
static bool vectors[IDT_MAX_DESCRIPTORS];

// General exception handler
void interrupt_handler(uint32_t interruptCode) {

    if (interruptCode < 32) {
        printf("CPU EXCEPTION: \n");
    }
    __asm__ volatile ("cli; hlt"); // Completely hangs the computer
}

void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags) {
    idt_entry_t* descriptor = &idt[vector];

    descriptor->isr_low        = (uint32_t)isr & 0xFFFF;
    descriptor->kernel_cs      = 0x08; // kernel code selector in GDT
    descriptor->attributes     = flags;
    descriptor->isr_high       = (uint32_t)isr >> 16;
    descriptor->reserved       = 0;
}


extern void* isr_stub_table[];

void initidt() {
    idtr.base = (uintptr_t)&idt[0];
    idtr.limit = (uint16_t)sizeof(idt_entry_t) * IDT_MAX_DESCRIPTORS - 1;

    for (uint8_t vector = 0; vector < 32; vector++) {
        idt_set_descriptor(vector, isr_stub_table[vector], 0x8E);
        vectors[vector] = true;
    }

    __asm__ volatile ("lidt %0" : : "m"(idtr)); // load the new IDT
    __asm__ volatile ("sti"); // set the interrupt flag

    PIC_remap(PIC1, PIC2);
}
