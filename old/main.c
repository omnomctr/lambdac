#include "arena.h"
#include "term.h"
#include "y.tab.h"

Arena *parser_arena;

int main(int argc, char *argv[])
{
    //extern void yyparse(void);
    parser_arena = arena_new(ARENA_DEFAULT_SIZE);
    yyparse();
    arena_destroy(parser_arena);
    return 0;
}
