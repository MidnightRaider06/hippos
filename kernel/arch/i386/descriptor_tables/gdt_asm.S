
.section .text
.global gdt_flush
.type gdt_flush, @function
gdt_flush:
	movl 4(%esp), %eax # Move pointer to gdt_ptr to eax
    lgdt (%eax)

    movl $0x10, %eax # Offset to kernel data segment
    movw %ax, %ds   # Set data segment registers
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    movw %ax, %ss
    ljmp $0x08, $_flush # Set cs to kernel code segment offset
_flush:
    ret
