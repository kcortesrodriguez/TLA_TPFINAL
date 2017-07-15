%define parse.error verbose
%locations

%{
#include <stdio.h>
#include <stdlib.h>

#include "joutput.h"
#include "handler.h"

extern int yylex();
void yyerror(const char *s);

int line;

%}

%union 
{
        int number;
        char *string;
}

%token RUN FUNCTION EXIT CONST
%token INTEGER_TYPE STRING_TYPE 
%token SET LINE CALL
%token IF FOR START END WHILE

%token <number> INTEGER
%token <string> ID
%token <string> TEXT

%right "="
%left OR AND
%left '>' '<' LE GE EQ NE
%left '+' '-'
%left '*' '/'
%right UMINUS
%left '!'
%start INIT

%%

INIT: 	RUN FUNS EXIT	 	{ 	
								return handle_main();
							}
		;

FUNS: 		FUN FUNS
			| FUN
			;

FUN:		FUNCTION INTEGER_TYPE ID '[' PARAMS ']' BODY 	{
																line = @1.first_line;
																handle_params_notSupported();
																return false;
															}
			
			| FUNCTION STRING_TYPE ID '[' PARAMS ']' BODY 	{
																line = @1.first_line;
																handle_params_notSupported();
																return false;
															}
			| FUNCTION INTEGER_TYPE ID BODY {
												line = @1.first_line;
												if (!handle_funDeclaration($3))
													return false;

											}

			| FUNCTION STRING_TYPE ID BODY	{
												line = @1.first_line;
												if (!handle_funDeclaration($3))
													return false;
											}

			;

PARAMS: 	INTEGER_TYPE ID PARAMS 	
									
			| STRING_TYPE ID PARAMS 
								
			| INTEGER_TYPE ID 		
									
			| STRING_TYPE ID 		
									
			;

STIF:		IF '[' BOOLEANS ']' BODY	{ 
											handle_if();
 										}
			;

STWHILE:	WHILE '[' BOOLEANS ']' BODY 	{ 
												handle_while();
			;								}

STFOR:		FOR COND_FOR BODY		{ 
										handle_for();
									}
			;

STREAD:		':' ID ':' '.' 	{
								line = @1.first_line; 
								if (!handle_stdRead($2))
									return false;
							}
			;

STCONST: 	CONST ID INTEGER '.'	{
										line = @1.first_line;
 										if (!handle_constInteger($2, $3))
 											return false;
 									}
 
STFUNCTION:	CALL ID '.'	{
							line = @1.first_line;
							if (!handle_funCall($2))
								return false;
						}	

STPRINT:	'&' ID '&''.'					 	{
													line = @1.first_line;
													if (!handle_var_stdWrite($2))
														return false;
												}
			
			| '&' INTEGER '&''.' 				{ 
													handle_int_stdWrite($2);
												}
			
			| '&' TEXT  '&''.'					{ 	
													handle_string_stdWrite($2);													
												}
			
			| '&' LINE '&''.'					{
													handle_string_stdWrite("System.lineSeparator()");
												}
			;

COND_FOR: 	'[' ID '=' INTEGER ':' INTEGER ']' 	{ 
													handle_condFor($2,$4,$6);
												}
			
			| '[' ID '=' INTEGER ':' ID ']' 	{ 	
													line = @1.first_line;
													if (!handle_condFor_varLimit($2,$4,$6))
														return false;

												}
			
			| '[' ID '=' ID ':' ID ']' 			{ 	
													line = @1.first_line;
													if (!handle_condFor_allVar($2, $4, $6))
														return false;
												}
			;

BODY:		START STATEMENTS END
			;

DECLARE_VAR: 	STRING_TYPE ID '=' TEXT '.'			{
														line = @1.first_line;
														if (!handle_string_varDeclaration($2, $4))	
															return false;
													}
													
				| INTEGER_TYPE ID '=' INT_OPS '.' 	{ 		
														line = @1.first_line;
														if (!handle_int_varDeclaration($2))
															return false;
													}
				;

CHANGE_VAR: 	SET ID INT_OPS '.'	{ 	
										line = @1.first_line;
										if (!handle_int_set($2))
											return false;
									}
				
				| SET ID TEXT '.'	{	
										line = @1.first_line;
										if (!handle_string_set($2, $3))
											return false;
									}
				;

STATEMENTS:		DECLARE_VAR
				| STIF
				| STFOR
				| STWHILE
				| STREAD
				| STPRINT
				| CHANGE_VAR
				| STFUNCTION
				| STCONST
				| DECLARE_VAR STATEMENTS
				| STIF STATEMENTS
				| STFOR STATEMENTS
				| STWHILE STATEMENTS
				| STREAD STATEMENTS
				| STPRINT STATEMENTS
				| CHANGE_VAR STATEMENTS
				| STFUNCTION STATEMENTS
				| STCONST STATEMENTS
				;

INT_OPS:		TYPOS '+' INT_OPS			{
												handle_int_op("+");
											}
				
				| TYPOS '-' INT_OPS			{
												handle_int_op("-");
											}
				
				| TYPOS '*' INT_OPS			{
												handle_int_op("*");
											}
				
				| TYPOS '/' INT_OPS			{
												handle_int_op("/");
											}
				
				| TYPOS '%' INT_OPS			{
												handle_int_op("%");
											}
				
				| '(' INT_OPS ')'			{
												handle_parenthesis();
											}

				| TYPOS
				
				;
				
TYPOS:			INTEGER 					{
												handle_int($1);
											}				

				| ID 						{
												handle_var($1);
											}
				;
       
BOOL_CONDITION: 
				INTEGER '>' INTEGER 			{
													handle_int_cmp(">", $1, $3);
												}
				
				| INTEGER '<' INTEGER  			{
													handle_int_cmp("<", $1, $3);
												}
				
				| INTEGER '=''=' INTEGER 		{
													handle_int_cmp("==", $1, $4);
												}
				
				| ID '>' INTEGER 				{	
													line = @1.first_line;			
													if (!handle_int_var_cmp(">", $1, $3))
														return false;
												}
				
				| ID '<' INTEGER 				{
													line = @1.first_line;			
													if (!handle_int_var_cmp("<", $1, $3))
														return false;
												} 
				
				| ID '=''=' INTEGER 			{
													line = @1.first_line;				
													if (!handle_int_var_cmp("==", $1, $4))
														return false;
												}
				
				| ID '>' ID 					{ 
													line = @1.first_line;				
													if (!handle_var_var_cmp(">", $1, $3))
														return false;
												} 
				
				| ID '<' ID 					{
													line = @1.first_line;				
													if (!handle_var_var_cmp("<", $1, $3))
														return false;
												} 
				
				| ID '=''=' ID 					{
													line = @1.first_line;				
													if (!handle_var_var_cmp("==", $1, $4))
														return false;
												}
				
				| INTEGER '>' ID 				{
													line = @1.first_line;				
													if (!handle_int_var_cmp(">", $3, $1))
														return false;
												}  
				
				| INTEGER '<' ID 				{
													line = @1.first_line;				
													if (!handle_int_var_cmp("<", $3, $1))
														return false;
												} 
				
				| INTEGER '=''=' ID 			{
													line = @1.first_line;				
													if (!handle_int_var_cmp("==", $4, $1))
														return false;
												}
				;		


BOOLEANS:	'!' '(' BOOL_CONDITION ')' 							{ createCMPAuxiliarNode("!"); } //quiza aca, en yacc en vez de trabajar con BOOL_CONDITION me convenga trabajar con BOOLEANS
			| '(' BOOL_CONDITION '&''&' BOOL_CONDITION ')'		{ createCMPAuxiliar2Node("&&"); }
			| '(' BOOL_CONDITION '|''|' BOOL_CONDITION ')'		{ createCMPAuxiliar2Node("||"); }
			| BOOL_CONDITION
			;

%%

int yywrap()
{
        return 1;
} 

int main() {

   	outputinit();
   	output("import java.util.Scanner;\n");
	output("public class output {\n");

	int valor = yyparse();

    if (valor != false){
    	to_ret_functions();
    } else {
    	output("public static void main(String[] args) {}");
    }

    output("}\n");
    outputfinish();  

} 