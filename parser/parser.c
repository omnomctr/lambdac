#include "parser.h"
#include <ctype.h>
#include <assert.h>

char *lexeme_kind_zstr(enum LexemeKind l);

Lexeme next_lexeme(struct Reader *r)
{
    int ch = readchar(r);
    while (isspace(ch)) { ch = readchar(r); }

    switch (ch) {
    case EOF: {
        return (Lexeme) { .kind = TOK_EOF };
    } break;
    case '\\': {
        return (Lexeme) { .kind = TOK_LAMBDA };
    } break;
    case '(': {
        return (Lexeme) { .kind = TOK_LPAREN };
    } break;
    case ')': {
        return (Lexeme) { .kind = TOK_RPAREN };
    } break;
    case '.': {
        return (Lexeme) { .kind = TOK_DOT };
    } break;
    default:
        if (isalpha(ch)) {
            return (Lexeme) { .kind = TOK_VAR, .as.var.ident = ch }; 
        } else {
            return (Lexeme) { .kind = TOK_ILLEGAL, .as.illegal.illegal = ch };
        }
    }
}

Parser *parser_new(Arena *a, struct Reader *r)
{
    Parser *ret = arena_alloc(a, sizeof(Parser));
    *ret = (Parser) {
        .reader = r,
        .current_lexeme = next_lexeme(r),
        .arena = a,
    };
    return ret;
}

static void p_next_lexeme(Parser *p)
{
    assert(p->current_lexeme.kind != TOK_EOF);
    p->current_lexeme = next_lexeme(p->reader);
}

#define EAT(p, _kind) \
    do { \
        if ((p)->current_lexeme.kind != (_kind)) { \
            fprintf(stderr, "expected lexeme of kind %s, got %s\n", lexeme_kind_zstr((_kind)), lexeme_kind_zstr((p)->current_lexeme.kind)); \
            return NULL; \
        } else p_next_lexeme(p); \
    } while (0);

Term *parse_term(Parser *p) 
{
    Term *ret = NULL;

    if (p->current_lexeme.kind == TOK_LAMBDA) {
        EAT(p, TOK_LAMBDA);
        if (p->current_lexeme.kind != TOK_VAR) {
            fprintf(stderr, "expected TOK_VAR, got %s\n", lexeme_kind_zstr(p->current_lexeme.kind));
            return NULL;
        }

        Term *arg = variable_new(p->arena, p->current_lexeme.as.var.ident);
        p_next_lexeme(p);

        EAT(p, TOK_DOT);
        Term *body = parse_term(p);
        if (body == NULL) return body;
        ret = lambda_new(p->arena, arg, body);
    } else if (p->current_lexeme.kind == TOK_VAR) {
        ret = variable_new(p->arena, p->current_lexeme.as.var.ident);
        p_next_lexeme(p);
    } else if (p->current_lexeme.kind == TOK_LPAREN) {
        p_next_lexeme(p);
        Term *lhs = parse_term(p);
        if (lhs == NULL) return lhs;

        while (p->current_lexeme.kind != TOK_RPAREN) {
            Term *rhs = parse_term(p);
            if (rhs == NULL) return rhs;
            lhs = application_new(p->arena, lhs, rhs);
        }
        EAT(p, TOK_RPAREN);
        ret = lhs;
    } else {
        fprintf(stderr, "unexpected lexeme %s\n", lexeme_kind_zstr(p->current_lexeme.kind));
    }

    return ret;
}

char *lexeme_kind_zstr(enum LexemeKind l)
{
    switch(l) {
    case TOK_LAMBDA: return "{lambda}";
    case TOK_LPAREN: return "{lparen}";
    case TOK_RPAREN: return "{rparen}";
    case TOK_DOT: return "{dot}";
    case TOK_VAR: return "{var}";
    case TOK_EOF: return "{EOF}";
    case TOK_ILLEGAL: return "{illegal}";
    }

    assert(0 && "infallible");
    return NULL;
}

#define DEBUG_TOOLS
#ifdef DEBUG_TOOLS

#include <stdio.h>

void lexeme_print(Lexeme l) 
{
    switch(l.kind) {
    case TOK_LAMBDA: {
        printf("{lambda}");
    } break;
    case TOK_LPAREN: {
        printf("{lparen}");
    } break;
    case TOK_RPAREN: {
        printf("{rparen}");
    } break;
    case TOK_DOT: {
        printf("{dot}");
    } break;
    case TOK_VAR: {
        printf("{var %c}", l.as.var.ident);
    } break;
    case TOK_EOF: {
        printf("{eof}");
    } break;
    case TOK_ILLEGAL: {
        printf("illegal");
    } break;
    }
}

#endif
