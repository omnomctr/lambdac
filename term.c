#include "term.h"
#include <assert.h>
#include <stdio.h>

Term *lambda_new(Arena *a, Term *arg, Term *body)
{
    assert(arg->kind == VARIABLE_TERM);
    
    Term *ret = arena_alloc(a, sizeof(*ret));
    ret->kind = LAMBDA_TERM;
    ret->as.lambda.arg = arg;
    ret->as.lambda.body = body;

    return ret;
}

Term *variable_new(Arena *a, char c)
{
    Term *ret = arena_alloc(a, sizeof(*ret));
    ret->kind = VARIABLE_TERM;
    ret->as.variable.c = c;
    
    return ret;
}

Term *application_new(Arena *a, Term *lhs, Term *rhs)
{
    Term *ret = arena_alloc(a, sizeof(*ret));
    ret->kind = APPLICATION_TERM;
    ret->as.application.lhs = lhs;
    ret->as.application.rhs = rhs;

    return ret;
}

void term_print(Term *t)
{
    switch (t->kind) {
    case LAMBDA_TERM: {
        putchar('(');
        printf("λ");
        term_print(t->as.lambda.arg);
        putchar('.');
        term_print(t->as.lambda.body);
        putchar(')');
    } break;
    case VARIABLE_TERM: {
        putchar(t->as.variable.c);
    } break;
    case APPLICATION_TERM: {
        putchar('(');
        term_print(t->as.application.lhs);
        putchar(' ');
        term_print(t->as.application.rhs);
        putchar(')');
    } break;
    }
}


