#ifndef _GDT_H
#define _GDT_H 1

#include <stddef.h>
#include <stdint.h>

typedef struct __attribute__((packed)) gdt_entry {
    uint16_t limit;      // bits 0–15 of limit
    uint16_t base_low;       // bits 0–15 of base
    uint8_t  base_mid;    // bits 16–23 of base
    uint8_t  access;         // access byte
    uint8_t  flags;    // flags first 4 bits, limit bits 16-19 last 4 bits
    uint8_t  base_high;      // bits 24–31 of base
} gdt_entry_t ;

typedef struct  __attribute__((packed)) gdt_ptr {
    uint16_t size;  //size of table in bytes minus 1
    uint32_t offset;  //linear address of the GDT
} gdt_ptr_t;

void initgdt();
void gdt_insert(uint32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t flags);

#endif
