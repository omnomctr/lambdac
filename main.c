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
    Arena *a = arena_new(ARENA_DEFAULT_SIZE);

    term_print(application_new(a, lambda_new(a, variable_new(a, 'x'), variable_new(a, 'x')), variable_new(a, 'y')));

    arena_destroy(a);
    putchar('\n');

    Arena *parser_arena = arena_new(1024);

    using_history();
    for (;;) {
        char *line = readline("λ> ");
        if (line == NULL) break;

        add_history(line);

        ZStrReader zstr_reader = zstrreader_new(line);
        Parser *p = parser_new(parser_arena, zstrreader_to_reader(&zstr_reader));
        Term *t = parse_term(p);
        if (t) {
            term_print(t);
            putchar('\n');
        }

        arena_clear(parser_arena);

        free(line);
    }

    arena_destroy(parser_arena);
    return 0;
}
