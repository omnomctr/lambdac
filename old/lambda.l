%{

#include "term.h"
#include "y.tab.h"
#include <stdio.h>

void yyerror(char *);

extern Arena *parser_arena;
%}
%%

[()\\\.] { return *yytext; }
[a-zA-Z] { yylval.val = variable_new(parser_arena, *yytext); return VAR; }
[ \t] ;
. { yyerror("invalid character"); }

%%

int yywrap(void) 
{
    return 1;
}
