#include <kernel/gdt.h>
#include <stdint.h>
#include <stdio.h>

extern void gdt_flush(gdt_ptr_t *ptr);

gdt_entry_t gdt_entries[5];
gdt_ptr_t gdt_ptr;

void initgdt() {
    gdt_ptr.size = (sizeof(gdt_entry_t) * 5) - 1;
    gdt_ptr.offset = &gdt_entries;

    gdt_insert(0, 0, 0, 0, 0);  //Null segment
    gdt_insert(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);   //Kernel code segment
    gdt_insert(2, 0, 0xFFFFFFFF, 0x92, 0xCF);   //Kernel data segment
    gdt_insert(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);   //User code segment
    gdt_insert(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);   //User data segment
    //gdt_insert(5, 0, 0, 0, 0);  //Task state segment

    gdt_flush(&gdt_ptr);
}

void gdt_insert(uint32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t flags) {
    gdt_entry_t *curr = &gdt_entries[num];

    curr->limit = limit & 0xFFFF;
    curr->base_low = base & 0xFFFF;
    curr->base_mid = (base >> 16) & 0xFF;
    curr->access = access;
    curr->flags = (limit >> 16) & 0xF;
    curr->flags |= (flags & 0xF0);
    curr->base_high = (base >> 24) & 0xFF;

}
