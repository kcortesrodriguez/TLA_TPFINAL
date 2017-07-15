#ifndef DMANAGER_H
#define DAMANGER_H

#define MAX_VARIABLES 100
#define MAX_FUNCTIONS 100

typedef enum{false, true} bool;

unsigned int create_hash(char * key);
bool var_exist(char * key);
bool add_variable(char* key);
void clear_vars();
void printIDNotFound(char* id);
bool check_function_exist(char* function_name);
bool add_function(char* function_name);
bool check_main_exist();
void printIDAlreadyCreated(char* id);
void to_ret_functions();
void printFunctionNotFound(char* function_name);


#endif