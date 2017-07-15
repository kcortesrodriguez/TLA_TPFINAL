#include <stdio.h>
#include <stdlib.h>

#include "stack.h"
#include "dmanager.h"

struct Node* stack[MAX_STACK_ELEMS];
int position = 0;

struct Node* functions[MAX_FUNCTIONS];
int p_functions = 0;

void add(struct Node* node){
	stack[position] = node;
	position++;
}

struct Node* pop(){
	position--;
	return stack[position];
}

int get_tot_stack(){
	return position;
}

void add_function_stack(struct Node* node){
	functions[p_functions] = node;
	p_functions++;
}

struct Node* pop_function_stack(){
	p_functions--;
	return functions[p_functions];
}

int get_tot_functions(){
	return p_functions;
}