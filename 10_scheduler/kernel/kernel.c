#include "kernel.h"

void _start() {
    free_mem_addr = 0x20000;
    global_id = 0;
    memory_limit = 0x40000;
    isr_install();
    irq_install();

    kprint("Type something, it will go through the kernel\n"
        "Type END to halt the CPU\n> ");
}

void user_input(char *input) {
    if (strcmp(input, "END") == 0) {
        kprint("Stopping the CPU. Bye!\n");
        asm volatile("hlt");
    } else if (strcmp(input, "ADD") == 0) {
	u32 *n0; n0 = (u32 *) 16; n0++;
	node *n1; n1 = NULL;
	kmalloc( 20, 0, (u32 *) &n1);
        n1->id = global_id++;
	n1->base = global_id*100;
	n1->size = global_id*10;
	n1->next = NULL;
	n1->previous = NULL;
	kprint("ADDED: ");
	char c[16];
	hex_to_ascii( (u32 ) n1, c);
	kprint( c);
	kprint( "  id:  ");
	hex_to_ascii( n1->id, c);
	kprint( c);
    } else if (strcmp(input, "ZOO") == 0) { 
	kprint("ZOO.\n");
    }

    kprint("You said: ");
    kprint(input);
    kprint("\n> ");
}
