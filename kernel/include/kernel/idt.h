#ifndef _IDT_H
#define _IDT_H 1

#include <stddef.h>
#include <stdint.h>

typedef struct __attribute__((packed)) {
	uint16_t    isr_low;      // The lower 16 bits of the ISR's address
	uint16_t    kernel_cs;    // The GDT segment selector that the CPU will load into CS before calling the ISR
	uint8_t     reserved;     // Set to zero
	uint8_t     attributes;   // Type and attributes; see the IDT page
	uint16_t    isr_high;     // The higher 16 bits of the ISR's address
} idt_entry_t;

typedef struct __attribute__((packed)){
	uint16_t	limit;
	uint32_t	base;
} idtr_t;

__attribute__((noreturn))
void exception_handler(void);

void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags);
void initidt(void);
void cpuException(uint8_t interruptCode);

#endif