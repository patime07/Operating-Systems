#include "kernel.h"

// This only runs once and from there on forth only interrupts will cause something to happen
// e.g. displaying to screen and taking input from keyboard
void _start() {
  //question 1: 
  kprint("Name: Fatima zahra Iguenfer\n");
  kprint("Class Section: 03\n");

// Initialize Global Variables and set up interrrupt handlers
// These variables are a vulnerability to OS security if someone can adjust
// the lower value in the kernel code (exploiting, for  example, an Intel address hack)
// than kernel memory and even kernel code can be overwritten
// https://www.wired.com/story/intel-lab-istare-hack-chips/
// See libc/globals.h for KMEM_START and KMEM_END values
// See libc/globals.h for UMEM_START and UMEM_END values
   kmem_addr = KMEM_START;
   kernel_mem_limit = KMEM_END;

   umem_addr = UMEM_START;
   user_mem_limit = UMEM_END;

   global_head = NULL;
   global_id = 0;


    isr_install();
    irq_install();

    kprint("Type something, it will go through the kernel\n"
        "Type HELP to list commands\n> ");

void print_list() {
    // This assumes you have a global linked list called `global_head` tracking allocations.
    node *current = global_head;
    while (current != NULL) {
      kprint("Block ID: ");
      kprint_int(current->id); // Assuming a function to print integers exists
      kprint(", Size: ");
      kprint_int(current->size);
      kprint("\n");
      current = current->next;
    }
}




}

// An interrupt calls this function to parse what was typed in, this is, essentially, your
// Command Line Interpreter for now.
void user_input(char *input) {
   static u32 delete_id = 0;           // static persistent variable for incrementing during test
   static node* umem_head = NULL;      // static persistent head variable for contiguous block allocations
    if (strcmp(input, "END") == 0) {
        kprint("Stopping the CPU. Bye!\n");
        asm volatile("hlt");
    } else if (strcmp(input, "ADD") == 0) {
      umem_head = add_node( umem_head, 0x10000, 0x100, true, global_id++);
    } else if (strcmp(input, "LIST") == 0) {
      kprint("***** FORWARD ****\n");
      print_list( umem_head, true);
      kprint("***** REVERSE ****\n");
      print_list( umem_head, false);
    } else if (strcmp(input, "SHORTLIST") == 0) {
      shortprint_list( umem_head, true);
      kprint("\n******************\n");
      shortprint_list( umem_head, false);
    } else if (strcmp(input, "PAGE") == 0) {
        u32 phys_addr = 0;
        u32 page = umalloc(0x4200, 0, &phys_addr);
        kprint_hex( "Page: ", page, 10);
        kprint_hex(", physical address: ", phys_addr, 10);
        kprint("\n");
    } else if (strcmp(input, "DELETE") == 0) {
       umem_head = remove_node_by_id( umem_head, delete_id++);
    } else if (strcmp(input, "INSERT") == 0) {
      node *new_node = create_node( 0x15000, 0x1100, true, global_id++);
      node *insert_point = find_id( umem_head, 3);
      umem_head = insert_node( umem_head, insert_point, new_node, true);
      new_node = create_node( 0x18000, 0x2100, true, global_id++);
      insert_point = find_id( umem_head, 5);
      umem_head = insert_node( umem_head, insert_point, new_node, false);
    } else if (strcmp(input, "SORTA") == 0) {
      umem_head = hacksort_list( umem_head, true);
    } else if (strcmp(input, "SORTD") == 0) {
      umem_head = hacksort_list( umem_head, false);
    } else if (strcmp(input, "SWAP") == 0) {
      node *n1 = find_id( umem_head, 1);
      node *n2 = find_id( umem_head, 5);
      node *n3 = find_id( umem_head, 3);
      node *n4 = find_id( umem_head, 7);
      swap_node_data( n1, n2);
      swap_node_data( n2, n3);
      swap_node_data( n3, n4);
    } else if (strcmp(input, "TEST") == 0) {
       char s1[10] = "ABCDFFGH\0";
       char s2[10] = "ABCDEGH\0";
       int x = strncmp( s1, s2, 5);
       kprint_hex( "STRNCMP: ", x, 16);
       kprint("\n");

       x = sstrlen( s2, 10);
       kprint_hex( "SSTRLEN: ", x, 10);
       kprint("\n");

       x = strlen( s2);
       kprint_hex( "STRLEN: ", x, 10);
       kprint("\n");
    } else if (strcmp(input, "HELP") == 0) {
       kprint("Current Commands: ADD, LIST, SHORTLIST, PAGE, DELETE,\n");
       kprint("                : END, INSERT, SORTA, SORTD, SWAP, TEST, HELP\n");
		kprint("   Review the kernel.c source code to see what each command does.\n");
		kprint("   These are hard coded and are just examples, modify as you see fit.\n");
		kprint(" for example - TEST was just added so that I could test the strlen commands.\n");
    } 