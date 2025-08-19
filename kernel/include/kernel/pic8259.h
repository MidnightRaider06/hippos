#pragma once

#include <stdint.h>

#define PIC1        0x20    // IO base address for master PIC
#define PIC2        0x28    // IO base address for slave PIC
#define PIC1_COMMAND PIC1
#define PIC1_DATA    (PIC1 + 1)
#define PIC2_COMMAND PIC2
#define PIC2_DATA    (PIC2 + 1)
#define PIC_EOI		0x20		/* End-of-interrupt command code */


/* INITIALIZATION COMMAND WORDS (ICW)

ICW1 – First command that says: "I'm starting initialization now!"

ICW2 – The interrupt vector offset, i.e., where in the IDT to start putting IRQs.

ICW3 – Describes how the master and slave PICs are connected (wiring info).

ICW4 – Gives additional information about the environment.
*/

#define ICW1_ICW4	0x01		/* Indicates that ICW4 will be present */
#define ICW1_SINGLE	0x02		/* Single (cascade) mode */
#define ICW1_INTERVAL4	0x04		/* Call address interval 4 (8) */
#define ICW1_LEVEL	0x08		/* Level triggered (edge) mode */
#define ICW1_INIT	0x10		/* Initialization - required! */

#define ICW4_8086	0x01		/* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO	0x02		/* Auto (normal) EOI */
#define ICW4_BUF_SLAVE	0x08		/* Buffered mode/slave */
#define ICW4_BUF_MASTER	0x0C		/* Buffered mode/master */
#define ICW4_SFNM	0x10		/* Special fully nested (not) */

#define CASCADE_IRQ 2

/* OPERATION COMMAND WORDS (OCW) */

#define PIC_READ_IRR 0x0a  // OCW3: command to read the Interrupt Request Register (IRR)
#define PIC_READ_ISR 0x0b  // OCW3: command to read the In-Service Register (ISR)

void PIC_remap(int offset1, int offset2);
void PIC_sendEOI(uint8_t irq);
void PIC_disable(void);
void IRQ_set_mask(uint8_t IRQline);
void IRQ_clear_mask(uint8_t IRQline);
uint16_t pic_get_irr(void);
uint16_t pic_get_isr(void);
void translatePicInterrupt(uint8_t interruptCode);