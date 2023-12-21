#include "mem.h"

void memory_copy(u8 *source, u8 *dest, int nbytes) {
    int i;
    for (i = 0; i < nbytes; i++) {
        *(dest + i) = *(source + i);
    }
}

void memory_set(u8 *dest, u8 val, u32 len) {
    u8 *temp = (u8 *)dest;
    for ( ; len != 0; len--) *temp++ = val;
}

// See mem.h for free_mem_addr global declaration
/* Implementation is just a pointer to some free memory which
 * keeps growing */
u32 kmalloc(u32 size, int align, u32 *phys_addr) {
    /* Pages are aligned to 4K, or 0x1000 */
    if (align == 1 && (free_mem_addr & 0xFFFFF000)) {
        free_mem_addr &= 0xFFFFF000;
        free_mem_addr += 0x1000;
    }


    char c[16];
    hex_to_ascii( (u32) *phys_addr, c);
    kprint("Inside kmalloc phys_addr = ");
    kprint(c);
    kprint("\n");

    /* Save also the physical address */
    *phys_addr = free_mem_addr;

    hex_to_ascii( (u32) *phys_addr, c);
    kprint("Inside kmalloc phys_addr = ");
    kprint(c);
    kprint("\n");

    u32 ret = free_mem_addr;
    free_mem_addr += size; /* Remember to increment the pointer */
    return ret;
}
