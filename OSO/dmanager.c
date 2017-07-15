#include <stdio.h>
#include <stdlib.h>
#include "dmanager.h"
#include "stack.h"
#include "joutput.h"

unsigned int vars_names[MAX_VARIABLES] = {0};
unsigned int functions_names[MAX_FUNCTIONS] = {0};

int tot_functions_defined = 0;
int tot_var_defined = 0;

unsigned int create_hash(char *key){
    unsigned int hash, i;

    int counter = 0;
    while(key[counter] != '\0'){
    	counter++;
    }

    for(hash = i = 0; i < counter; ++i){
        hash += key[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }

    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash;
}

bool var_exist(char * id){

	unsigned int hash_var_name = create_hash(id);

	int i;
	for(i = 0 ; i < tot_var_defined; i++){
		if( vars_names[i] == hash_var_name ){
			return true;
		}
	}
	return false;
}

bool add_variable(char* key){

	if(!var_exist(key)){
		vars_names[tot_var_defined] = create_hash(key);
		tot_var_defined++;
		return true;
	}else
		return false;
}

void clear_vars(){
	tot_var_defined = 0;
}

bool check_function_exist(char* function_name){
	int hash = create_hash(function_name);

	int i;
	for( i = 0 ; i < tot_functions_defined ; i++){
		if (functions_names[i] == hash){
			return true;
		}
	}

	return false;

}

bool add_function(char* function_name){

	if (check_function_exist(function_name)){
		return false;
	}

	int hash = create_hash(function_name);
	functions_names[tot_functions_defined] = hash;
	tot_functions_defined++;
	return true;

}

bool check_main_exist(){
	int i = 0;
	int hash_main = create_hash("main");

	for( i = 0 ; i < tot_functions_defined ; i++){
		if( functions_names[i] == hash_main )
			return true;
	}
	return false;
}

void to_ret_functions(){
	int c = get_tot_functions();

	int i;
	for(i = 0 ; i < c ; i++)
		output(pop_function_stack()->jconv);
}