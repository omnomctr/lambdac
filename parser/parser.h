#ifndef PARSER_HEADER__
#define PARSER_HEADER__

#include "reader.h"
#include "../arena.h"
#include "../term.h"

enum LexemeKind {
   TOK_LAMBDA = 1,
   TOK_LPAREN = 2,
   TOK_RPAREN = 3,
   TOK_DOT = 4,
   TOK_VAR = 5,
   TOK_EOF = 6,
   TOK_ILLEGAL = 7,
};

typedef struct Lexeme {
    enum LexemeKind kind;
    union {
        struct {
            char ident;
        } var;
        struct {
            char illegal;
        } illegal;
    } as;
} Lexeme;

Lexeme next_lexeme(struct Reader *r);
void lexeme_print(Lexeme l);

typedef struct Parser {
    struct Reader *reader;
    Lexeme current_lexeme;
    Arena *arena;
} Parser;

Parser *parser_new(Arena *a, struct Reader *r);
Term *parse_term(Parser *p);

#endif
