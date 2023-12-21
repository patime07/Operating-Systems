#ifndef MEM_H
#define MEM_H

#include "../cpu/types.h"
#include "../libc/string.h"
#include "../drivers/screen.h"
#include "globals.h"

typedef struct _node {
	u32 id;
	u32 base;
	u32 size;
	struct _node *next;
	struct _node *previous;
} node;

void memory_copy(u8 *source, u8 *dest, int nbytes);
void memory_set(u8 *dest, u8 val, u32 len);

u32 kmalloc(u32 size, int align, u32 *phys_addr);

#endif
