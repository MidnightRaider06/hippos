#include <kernel/pic8259.h>
#include <kernel/io.h>

/**
 * Remaps the PIC interrupt vectors.
 *
 * The default IRQ vector offsets used by the master and slave PICs conflict
 * with CPU exception vectors (0x00–0x1F). This function remaps the PICs to use
 * non-conflicting vector offsets starting at `offset1` for the master PIC and
 * `offset2` for the slave PIC.
 *
 * The remapping process involves sending a specific sequence of initialization
 * control words (ICWs) to both PICs and unmasking all IRQs afterward.
 *
 * offset1 : New vector offset for IRQ0–IRQ7 (master PIC).
 * offset2 : New vector offset for IRQ8–IRQ15 (slave PIC).
 */
void PIC_remap(int offset1, int offset2)
{
	outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);  // starts the initialization sequence (in cascade mode)
	io_wait();
	outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
	io_wait();
	outb(PIC1_DATA, offset1);                 // ICW2: Master PIC vector offset
	io_wait();
	outb(PIC2_DATA, offset2);                 // ICW2: Slave PIC vector offset
	io_wait();
	outb(PIC1_DATA, 1 << CASCADE_IRQ);        // ICW3: tell Master PIC that there is a slave PIC at IRQ2
	io_wait();
	outb(PIC2_DATA, 2);                       // ICW3: tell Slave PIC its cascade identity (0000 0010)
	io_wait();
	
	outb(PIC1_DATA, ICW4_8086);               // ICW4: have the PICs use 8086 mode
	io_wait();
	outb(PIC2_DATA, ICW4_8086);
	io_wait();

	// Unmask both PICs.
	outb(PIC1_DATA, 0);
	outb(PIC2_DATA, 0);
}

/**
 * Sends an End of Interrupt (EOI) signal to the PICs.
 *
 * After an IRQ has been handled, an EOI must be sent to the PICs to tell them
 * that the interrupt has been serviced and they may issue more interrupts.
 *
 * If the IRQ originated from the slave PIC (IRQ 8–15), EOIs must be sent to
 * both the slave and master PICs. Otherwise, only the master PIC is notified.
 *
 * irq : The IRQ number that was handled (0–15).
 */
void PIC_sendEOI(uint8_t irq)
{
	if(irq >= 8)
		outb(PIC2_COMMAND,PIC_EOI);
	
	outb(PIC1_COMMAND,PIC_EOI);
}

/**
 * Masks (disables) all hardware interrupts on both PICs.
 *
 * This is typically used when transitioning to using the Advanced Programmable
 * Interrupt Controller (APIC) or I/O APIC, where the legacy PIC is no longer
 * needed to handle interrupts. All 16 IRQ lines are masked by setting every
 * bit in the Interrupt Mask Registers (IMRs).
 */
void PIC_disable(void) {
    outb(PIC1_DATA, 0xff);
    outb(PIC2_DATA, 0xff);
}

/*
 * Masks (disables) the specified IRQ line.
 *
 * This function sets the corresponding bit in the Interrupt Mask Register (IMR)
 * of either the master or slave PIC to disable the given IRQ line.
 *
 * IRQline : The IRQ line number (0–15) to be masked.
 */
void IRQ_set_mask(uint8_t IRQline) {
    uint16_t port;
    uint8_t value;

    if(IRQline < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        IRQline -= 8;
    }
    value = inb(port) | (1 << IRQline);
    outb(port, value);        
}

/*
 * Unasks (enables) the specified IRQ line.
 *
 * This function zeroes the corresponding bit in the Interrupt Mask Register (IMR)
 * of either the master or slave PIC to disable the given IRQ line.
 *
 * IRQline : The IRQ line number (0–15) to be masked.
 */
void IRQ_clear_mask(uint8_t IRQline) {
    uint16_t port;
    uint8_t value;

    if(IRQline < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        IRQline -= 8;
    }
    value = inb(port) & ~(1 << IRQline);
    outb(port, value);        
}

/* Helper func */
static uint16_t __pic_get_irq_reg(int ocw3);
uint16_t __pic_get_irq_reg(int ocw3)
{
    /* OCW3 to PIC CMD to get the register values.  PIC2 is chained, and
     * represents IRQs 8-15.  PIC1 is IRQs 0-7, with 2 being the chain */
    outb(PIC1_COMMAND, ocw3);
    outb(PIC2_COMMAND, ocw3);
    return (inb(PIC2_COMMAND) << 8) | inb(PIC1_COMMAND);
}

/* Returns the combined value of the cascaded PICs irq request register */
uint16_t pic_get_irr(void)
{
    return __pic_get_irq_reg(PIC_READ_IRR);
}

/* Returns the combined value of the cascaded PICs in-service register */
uint16_t pic_get_isr(void)
{
    return __pic_get_irq_reg(PIC_READ_ISR);
}
