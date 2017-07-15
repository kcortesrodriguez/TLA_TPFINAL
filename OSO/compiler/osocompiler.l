%option yylineno

%{
	#define YY_USER_ACTION yylloc.first_line = yylloc.last_line = yylineno;
	#include <stdio.h>
	#include <string.h>
    #include "y.tab.h"
%}

alpha [A-Za-z]
digit [0-9]

%%

[\t \n]

RUNOSO		return RUN;
EXITOSO		return EXIT;
for			return FOR;
if			return IF;
while 		return WHILE;
function	return FUNCTION;

integer		return INTEGER_TYPE;
string		return STRING_TYPE;
line		return LINE;
set 		return SET;
call		return CALL;
const 		return CONST;

start 		return START;
end			return END;


{digit}+	{
			yylval.number = atoi(yytext);
			return INTEGER;
			}

[a-zA-Z][a-zA-Z0-9]* 	{
						yylval.string = strdup(yytext);
						return ID;
						}

["][a-zA-Z0-9?! ]*["]	{
						yylval.string = strdup(yytext);
						return TEXT;
						}

.			return yytext[0];

%%

void yyerror(const char *str)
{
    fprintf(stderr,"Error at line %d : %s\n",yylineno, str);
}
