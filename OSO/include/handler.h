#ifndef HANDLER_H
#define HANDLER_H

#include "dmanager.h"

bool handle_main();
bool handle_funDeclaration(char * id);
bool handle_string_varDeclaration(char * id, char * string);
bool handle_int_varDeclaration(char * id);
bool handle_int_set(char * id);
bool handle_string_set(char * id, char * string);
bool handle_funCall(char * id);
bool handle_stdRead(char * id);
bool handle_var_stdWrite(char * id);
void handle_int_stdWrite(int value);
void handle_string_stdWrite(char * string);
bool handle_constInteger(char * id, int value);
void handle_int_op(char * op);
void handle_parenthesis();
void handle_if();
void handle_while();
void handle_for();
void handle_condFor(char * id, int value, int limit);
bool handle_condFor_varLimit(char * id, int value, char * var_limit);
bool handle_condFor_allVar(char * id, char * var_value, char * var_limit);
void handle_int(int value);
bool handle_var(char * id);
void handle_int_cmp(char * logic_op, int val1, int val2);
bool handle_int_var_cmp(char * logic_op, char * id, int value);
bool handle_var_var_cmp(char * logic_op, char * id1, char * id2);
void handle_params_notSupported();
void handle_fun_undefined(char* function_name);
void handle_funRep(char * id);
void handle_var_undefined(char* function_name);
void handle_varRep(char* id);
void handle_compile_success();
void handle_main_missing();

#endif