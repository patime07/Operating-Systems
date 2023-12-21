#ifndef LINKED_H
#define LINKED_H

#include "../cpu/types.h"
#include "globals.h"
#include "mem.h"

node *create_node( u32 base_register, u32 limit_register, bool ft_hole_mem, u32 id);
node *add_node( node *head, u32 base_register, u32 limit_register, bool ft_hole_mem, u32 id);
node *_add_node( node *head, node *new_node);
node *append_node( node *head, node* new_node);
node *find_id( node *head, u32 id);
node *remove_node_by_id( node *head, u32 id);
node *zip_list( node *head, node* target);
node *insert_node( node *head, node* insert_point, node *new_node, bool ft_before_after);
node *get_tail( node *head);
void get_min_max_id( node *head, u32 *min, u32 *max);
node *swap_nodes( node *head, node **left, node **right);
void swap_node_data( node* left, node *right);
node *hacksort_list( node* head, bool ft_backward_forward);
node *mergesort_list( node* head, node *pivot, node *left, node *right);
void free_node( node *target);
void print_node( node *current);
void print_list( node *head, bool ft_backward_forward);
void shortprint_list( node *head, bool ft_backward_forward);

#endif
