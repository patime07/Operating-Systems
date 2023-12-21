// linked.c   ANSI C kernel environment linked list
// requires kmalloc to allocate space for linked list

#include "linked.h"

// Create a blank node using kmalloc, then use allocated memory to store
// a linked list node with the values passed in the parameters
// You must capture the node returned with an lval result variable
node *create_node( u32 base_register, u32 limit_register, bool ft_hole_mem, u32 id)
{
   node *new_node = NULL;

	new_node = (node *) kmalloc( NODE_SIZE, 0, (u32 *) new_node);
	new_node->id = id;
	new_node->base_register = base_register;
	new_node->limit_register = limit_register * (new_node->id + 1);
	new_node->ft_hole_mem = ft_hole_mem;
	new_node->next = NULL;
	new_node->previous = NULL;
   return( new_node);
}

// Creates a node from parameters using the above and then passes it to insert the node into the linked list
// changes to *head will only affect a local stack variable
// You must capture the new head returned with an lval result variable to update head changes
// ft_hole_mem is not used currently but is included to maintain a "holes" list
// true means update memory list,  false means update holes list (if you were to use it)
node *add_node( node *head, u32 base_register, u32 limit_register, bool ft_hole_mem, u32 id)
{
   node *new_node = NULL;

   new_node = create_node( base_register, limit_register, ft_hole_mem, id);
   head = _add_node( head, new_node);
   return( head);
}

// place an already created node at the end of the list
// changes to *head will only affect a local stack variable
// You must capture the new head returned with an lval result variable to update head changes
node *_add_node( node *head, node *new_node)
{
	node *iterator = NULL;
   node *previous = NULL;

   if( new_node == NULL) {
      return(head);
   }
	if( head == NULL) {
		head = new_node;
		return ( head );
	}  // Stop here is head is empty and just set to new and return new head.

	iterator = head->next;   // Head is not NULL so continue with next node as iterator
	while( iterator != NULL) {
      previous = iterator;
		iterator = iterator->next;
	} // until iterator is null keep pointing new node back to previous node and iterator to next node

	if( previous == NULL) { // just head existed, append new node ...
		new_node->previous = head;
      head->next = new_node;
	} else {    // iterated to end of list, append new node ...
      new_node->previous = previous;
      previous->next = new_node;
   } // ... and return
	return( head);
}

// Iterate to last node and list and then append new node to it
node *append_node( node *head, node* new_node)
{
   node *iterator = head;

   if( new_node == NULL) {
      return( head);
   }
   if( head == NULL) {
      return( new_node);
   }
   while( iterator->next != NULL) {  // step until at last node
      iterator = iterator->next;
   }
   iterator->next = new_node;
   new_node->previous = iterator;
   new_node->next = NULL;
   return(head);
}
      
// Given an input id search list until id found
// return lval of that *node value otherwise return a NULL value
node *find_id( node *head, u32 id)
{
   if( head == NULL) {
      return( head);
   }
   node *iterator = head;

   while(( iterator != NULL) && ( iterator->id != id)) {  // search and compare id
      iterator = iterator->next;
   }
   return( iterator);
}
 
// Given an id, find it with the function above and then use node* value (target)
// to create a gap of previous and next nodes which is then closed.
// if id is not found nothing happens
node *remove_node_by_id( node *head, u32 id)
{
   node *target = NULL;

   target = find_id( head, id);
   if( target == NULL) {
      return( head);
   }
   head = zip_list( head, target);
   free_node( target);
   return( head);
}

// Given a specific lval node* target value, remove it from the list *head
// and then close the gap created by removing target.
node *zip_list( node *head, node* target)
{
   bool has_previous = true;
   bool has_next = true;

   if( target == NULL) {
      return( head);
   }
   if( target->previous == NULL) {
      has_previous = false; 
#ifdef KDEBUG
      kprint("has_previous FALSE\n");
#endif
   }
   if ( target->next == NULL) {
      has_next = false;
#ifdef KDEBUG
      kprint("has_next FALSE\n");
#endif
   }
   if( (has_next == true) && (has_previous == true)) {      // Node in middle of chain
      target->previous->next = target->next;
      target->next->previous = target->previous;
   } else if ( (has_next == false && has_previous == true)) {  // Node at end of chain
      target->previous->next = NULL;
   } else if ( (has_next == true && has_previous == false)) {  // Node at start of chain
      target->next->previous = NULL;
      head = head->next;
   } else { // ( has_next == false && has_previous == false)    // Node is isolated
      head = NULL;
   }
   return(head);
}

// Use find_id to get the insert_point
// decide if insertion is before or after
// if ft_before_after = FALSE then insert before target
// if ft_before_after = TRUE then append after target
node *insert_node( node *head, node* insert_point, node *new_node, bool ft_before_after)
{
   if( new_node == NULL) {  // Nothing to add so return unchanged
      return( head);
   }
   if( insert_point == NULL) {  // If no point specified either place before head or after tail
      if( ft_before_after) {    // if after is set append as new tail
         head = append_node( head, new_node);
      } else {                   // if before than insert before head as new head
         new_node->next = head;
         new_node->previous = NULL;
         head-> previous = new_node;
         return( new_node);      // return new head
      }
      return( head);
   }

   if( ft_before_after) {   // if adding after then...
      if( insert_point->next != NULL) {  // insert node after insertion_point
         insert_point->next->previous = new_node;
         new_node->next = insert_point->next;
      } else {  // insertion point is last node instead, skip above operations
         new_node->next = NULL;
      }
      new_node->previous = insert_point;
      insert_point->next = new_node;
   } else {
      if( insert_point->previous != NULL) { // insert node before insertion point
         insert_point->previous->next = new_node;
         new_node->previous = insert_point->previous;
      } else { // insertion point is head
         new_node->previous = NULL;
         head = new_node;   // set new head as insertion point
      }
      insert_point->previous = new_node;
      new_node->next = insert_point;
   }
   return(head);  // return changes (if any) to head
}

node *get_tail( node *head)
{
   if( head == NULL) {
      return(head);
   }
   node *tail = head;
   while( tail->next != NULL) {  // iterate until the next node is not NULL
      tail = tail->next;
   }
   return( tail);
}

void get_min_max_id( node *head, u32 *min, u32 *max)
{
   if( head == NULL) {
      return;
   }
   node *iterator = head;
   *min = 0xFFFFFFFF;
   *max = 0;

   while( iterator != NULL) {  // iterate until the next node is not NULL
      if( *min > iterator->id) {
         *min = iterator->id;
      }
      if( *max < iterator->id) {
         *max = iterator->id;
      }
      iterator = iterator->next;
   }
   return;
}

void swap_node_data( node* left, node *right)
{
	if(( left == NULL) || (right == NULL)) {
		kprint("Invalid element.\n");
		return;
	}
   node swap;

   kprint_hex("SWAP ", left->id, 4);
   kprint_hex(" with ", right->id, 4);
   kprint("\n");

   swap.id = left->id;
   swap.base_register = left->base_register;
   swap.limit_register = left->limit_register;
   swap.ft_hole_mem = left->ft_hole_mem;

   left->id = right->id;
   left->base_register = right->base_register;
   left->limit_register = right->limit_register;
   left->ft_hole_mem = right->ft_hole_mem;

   right->id = swap.id;
   right->base_register = swap.base_register;
   right->limit_register = swap.limit_register;
   right->ft_hole_mem = swap.ft_hole_mem;

   return;
}

node *swap_nodes( node *head, node** left, node **right)
{

   if( head == NULL) {   // don't swap nodes in a list that is empty
      return( head);
   }

   if( (*left == NULL) || (*right == NULL) ) {   // if either or both nodes are NULL do nothing
      return( head);
   }
   if ( head == *left) {      // if left happens to be head than set head to right
      head = *right;
   }
   if ( head == *right) {     // if right happens to be head then set head to left
      head = *left;
   }
      
   node *left_previous = (*left)->previous;
   node *left_next = (*left)->next;
   node *right_previous = (*right)->previous;
   node *right_next = (*right)->next;

   if( (*left)->previous != NULL) {    // make sure not to access a NULL value node
      (*left)->previous->next = *right;    // previous node to left now points to right instead of left
   }

   if( (*left)->next != NULL) {     // same
      (*left)->next->previous = *right;   // next node to left now points to right instead of left
   }

   if( (*right)->previous != NULL) {   // same
      (*right)->previous->next = *left;    // same as above but right previous to left now
   }
   if( (*right)->next != NULL) {       // same
      (*right)->next->previous = *left;    // same as above...
   }

   (*left)->previous = right_previous; // this will obliterate left->previous so it is preserved in left_previous
   (*left)->next = right_next;         // this will obliterate left->next so it is preserved in left_next

   (*right)->previous = left_previous; // now point right node back to previous to left node (obliterated above)
   (*right)->next = left_next;         // now point right node forward to next of left node (obliterated above)

	node* swap;
	swap = *left;
	*left = *right;
	*right = swap;

#ifdef KDEBUG
	kprint("LEFT: ");
	print_node( *left);
	kprint("     RIGHT: ");
	print_node( *right);
	kprint("\n");
#endif

   return( head);    // return new head if it changed
}

// good ways to sort a list, transfer it to an array and merge or quicksort it
// not so good bubble-sort, insertion, select-sort, heap-sort
// worst, this ugly hack
node *hacksort_list( node* head, bool ft_descending_ascending)
{
   if( head == NULL) {
      kprint("LIST EMPTY\n");
      return(head);
   }
   if( ft_descending_ascending) {
      kprint("ASCENDING\n");
   } else {
      kprint("DESCENDING\n");
   }
   node* outer_iterator = head;
   node* inner_iterator = head->next;
   node* max = head;

   while( outer_iterator != NULL) {
      max = outer_iterator;
      inner_iterator = outer_iterator->next;
      while( inner_iterator != NULL) {
         if( ft_descending_ascending) {
            if( max->id > inner_iterator->id) {    // This decides ASCENDING
               max = inner_iterator;
            }
         } else {
            if( max->id < inner_iterator->id) {   // This decides SORT DESCENDING
               max = inner_iterator;
            }
         }
         inner_iterator = inner_iterator->next;
/*******  STOP COUNTER for Infnite Loop Halting
   u32 count = 0;
 if( count++ > 10) {
   return(head);
 }
****/
      }
      if( ( outer_iterator != NULL) && ( outer_iterator != max)) {
         swap_node_data( outer_iterator, max);
      }
      outer_iterator = outer_iterator->next;
   }
   return( head);
}
   
node *mergesort_list( node* head, node *pivot, node *left, node *right)
{
   return( head);  // disabled, not yet implemented
   if( ( head == NULL) || ( pivot == NULL) | ( left == NULL) | ( right == NULL)) {
      return( head);
   }
   node *start = left;
   node *end = right;
   while( (start->next != NULL) || (start->id != end->id)) {
      if( start->id < pivot->id) {
         ;
      }
   }
   
   return( head);
}

// stand in for delete, this only zeroes all the values of the node allocated by kmalloc
// the memory is not reclaimed, need to add a tracker for these kmalloc allocated blocks
// to implement a proper delete function
void free_node( node *target)
{
   memory_set( (u8 *) target, 0, NODE_SIZE);
   target = NULL;
   return;
}

// Print a single node's values
void print_node( node *current)
{ 
   char numstr[16];

	if( current->previous != NULL) {
		kprint_hex( "(", current->previous->id, 4);
		kprint( ")<-");
	} else {
		kprint("NULL<-");
	}

   memory_set( (u8 *) numstr, 0, 16);
   hex_to_ascii( current->id, numstr, 16);
   kprint("ID: "); kprint( numstr); kprint(" --- ");
   memory_set( (u8 *) numstr, 0, 16);
   hex_to_ascii( current->base_register, numstr, 16);
   kprint("BASE: "); kprint( numstr); kprint(" --- ");
   memory_set( (u8 *) numstr, 0, 16);
   hex_to_ascii( current->limit_register, numstr, 16);
   kprint("LIMIT: "); kprint( numstr); kprint(" --- ");
   if( current->ft_hole_mem) {
      kprint("MEMORY ");
   } else {
      kprint("HOLE  ");
   }
	if( current->next != NULL) {
		kprint_hex( "->", current->next->id, 4);
	} else {
		kprint("->NULL");
	}
	kprint("\n");
   return;
}

// just print the id separated by commas with a line-feed at the end
void shortprint_list( node *head, bool ft_descending_ascending) 
{
   if( head == NULL) {
      kprint( "EMPTY.");
      kprint("\n");
      return;
   }
   node *iterator = NULL;
   if( ft_descending_ascending) {
      iterator = head;
      while( iterator != NULL) {
         kprint_hex( ",",iterator->id, 4);
         iterator = iterator->next;
      }
   } else {
      iterator = get_tail( head);
      while( iterator != NULL) {
         kprint_hex( ",",iterator->id, 4);
         iterator = iterator->previous;
      }
   }
   kprint("\n");
   return;
}

// iterate through a list printing out the values of each node
void print_list( node *head, bool ft_descending_ascending)
{
   if( head == NULL) {
      kprint( "EMPTY.");
      return;
   }
   node *iterator = NULL;
   if( ft_descending_ascending) {
      iterator = head;
      while( iterator != NULL) {
         print_node( iterator);
         iterator = iterator->next;
      }
   } else {
      iterator = get_tail( head);
      while( iterator != NULL) {
         print_node( iterator);
         iterator = iterator->previous;
      }
   }
   return;
}
