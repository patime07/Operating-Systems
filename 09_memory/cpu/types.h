#ifndef TYPES_H
#define TYPES_H

/* Instead of using 'chars' to allocate non-character bytes,
 * we will use these new type with no semantic meaning */
typedef unsigned int   u32;
typedef          int   s32;
typedef unsigned short u16;
typedef          short s16;
typedef unsigned char  u8;
typedef          char  s8;

#define bool   u32
#define true   1
#define TRUE   1
#define false  0
#define FALSE  1

#define NULL ((void *) 0)
#define low_16(address) (u16)((address) & 0xFFFF)
#define high_16(address) (u16)(((address) >> 16) & 0xFFFF)

typedef struct _node {
   u32 id;              // generic unique id
   u32 base_register;   // the memory address start of allocation
   u32 limit_register;  // the size in bytes of the allocation
   bool ft_hole_mem;    // FALSE = hole, TRUE = memory allocation

   struct _node *next;
   struct _node *previous;
   // current size 6 words = 24bytes
} node;
#define NODE_SIZE 24

#endif
