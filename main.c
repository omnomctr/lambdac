#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <assert.h>
#include "arena.h"
#include "term.h"
#include "parser/reader.h"
#include "parser/parser.h"

int main(int argc, char *argv[]) 
{
    Arena *parser_arena = arena_new(1024);

    using_history();
    rl_variable_bind("blink-matching-paren", "On");
    for (;;) {
        char *line = readline("λ> ");
        if (line == NULL) break;

        add_history(line);

        ZStrReader reader = zstrreader_new(line);
        Parser *p = parser_new(parser_arena, zstrreader_to_reader(&reader));
        Term *t = parse_term(p);
        if (t) {
            printf("lambda term:\n\t");
            term_print(t);
            putchar('\n');

            term_finalize(t);
            printf("finalized w/ de bruijn indexing:\n\t");
            term_print(t);
            putchar('\n');
        }

        arena_clear(parser_arena);
        free(line);
    }

    arena_destroy(parser_arena);
    return 0;
}
