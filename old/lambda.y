%{

#include "term.h"
#include <stdio.h>
#include "arena.h"

extern Arena *parser_arena;

void yyerror(char *s);
int yylex(void);

%}

%union {
    Term *val;
}
%token <val> VAR

%type <val> term

%start list 

%%

list: 
    | list stat '\n'
    ;

stat: term { term_print($1); putchar('\n'); }
    ;

term: '\\' VAR '.' term { 
        $$ = lambda_new(parser_arena, $2, $4);
    }
    | VAR { $$ = $1; }
    | '(' term term ')' { $$ = application_new(parser_arena, $2, $3); }
    ;

%%

void yyerror(char *s)
{
    fprintf(stderr, "ERROR: %s\n", s);
}

#if 0

int main(int argc, char **argv) 
{
    parser_arena = arena_new(ARENA_DEFAULT_SIZE);
    yyparse();
    arena_free(parser_arena);
    return 0;
}

#endif
