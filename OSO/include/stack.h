#ifndef STACK_H
#define STACK_H

#define MAX_STACK_ELEMS 500
#define EMPTY_STACK 13

#include "tree.h"

void add(struct Node* node);
struct Node* pop();
void printStack();
int get_tot_stack();
void add_function_stack(struct Node* node);
struct Node* pop_function_stack();
int get_tot_functions();

#endif