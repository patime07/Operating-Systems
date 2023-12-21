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

// See globals.h for kmem_addr global declaration
// kmem_addr is reserved block of memory for kernel to store linked list nodes
/* Implementation is just a pointer to some free memory which
 * keeps growing */
u32 kmalloc(u32 size, int align, u32 *phys_addr) {
    /* Pages are aligned to 4K, or 0x1000 */
    if (align == 1 && (kmem_addr & 0xFFFFF000)) {
        kmem_addr &= 0xFFFFF000;
        kmem_addr += 0x1000;
    }


    char c[16];
    hex_to_ascii( *phys_addr, c, 16);
    kprint("Inside kmalloc phys_addr (before) = ");
    kprint(c);
    kprint(" --- ");

    /* Save the physical address */
    *phys_addr = kmem_addr;

    hex_to_ascii( *phys_addr, c, 16);
    kprint("(after) phys_addr = ");
    kprint(c);
    kprint("\n");

    u32 ret = kmem_addr;
    kmem_addr += size; /* Remember to increment the pointer */
    return ret;
}


// See globals.h for user_start global declaration
// umem_addr is reserved block of memory for kernel to store linked list nodes
/* Implementation is just a pointer to some free memory which
 * keeps growing */
u32 umalloc(u32 size, int align, u32 *phys_addr) {
    /* Pages are aligned to 4K, or 0x1000 */
    if (align == 1 && (umem_addr & 0xFFFFF000)) {
        umem_addr &= 0xFFFFF000;
        umem_addr += 0x1000;
    }


    char c[16];
    hex_to_ascii( *phys_addr, c, 16);
    kprint("Inside umalloc phys_addr (before) = ");
    kprint(c);
    kprint(" --- ");

    /* Save the physical address */
    *phys_addr = umem_addr;

    hex_to_ascii( *phys_addr, c, 16);
    kprint("(after) phys_addr = ");
    kprint(c);
    kprint("\n");

    u32 ret = umem_addr;
    umem_addr += size; /* Remember to increment the pointer */
    return ret;
}
