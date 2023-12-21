#ifndef GLOBAL_H
#define GLOBAL_H

#include "../cpu/types.h"
#include "linked.h"

// Using defines allows this file to control the ranges here rather than updating the kernel code lines
// somewhere else at various locations throughout the code
// using ALL CAPS is a conventions for defines and constants, that varies from group to group
#define KMEM_START 0x9000       // starts equal to this address for allocation
#define KMEM_END 0x10000       // cannot be equal to or more than this address
extern u32 kmem_addr;     // The start value of allocation kernel datastructure allocations
extern u32 kernel_mem_limit;      // the upper limit of memory allowed for kernel memory allocations

#define UMEM_START 0x10000   // kernel memory for linked list nodes starts here
#define UMEM_END 0x40000    // kernel memory for linked list nodes must end before here
extern u32 umem_addr;      // start of linked list memory allocation address
extern u32 user_mem_limit;        // end of linked list memory allocation address

extern u32 global_id;         // Just an id for now but will eventually become PID

extern node *global_head;            // Kernel data structure for storing memory allocation linked list
#endif // GLOBAL_H
