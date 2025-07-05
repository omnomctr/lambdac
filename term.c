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
    ret->finalized = false;

    return ret;
}

Term *variable_new(Arena *a, char c)
{
    Term *ret = arena_alloc(a, sizeof(*ret));
    ret->kind = VARIABLE_TERM;
    ret->as.variable.c = c;
    ret->finalized = false;
    
    return ret;
}

Term *application_new(Arena *a, Term *lhs, Term *rhs)
{
    Term *ret = arena_alloc(a, sizeof(*ret));
    ret->kind = APPLICATION_TERM;
    ret->as.application.lhs = lhs;
    ret->as.application.rhs = rhs;
    ret->finalized = false;

    return ret;
}

bool term_equal(Term *lhs, Term *rhs)
{
    if (lhs->kind != rhs->kind) return false;
    assert(lhs->finalized == rhs->finalized && "both terms must be in the same stage IE finalized");

    switch (lhs->kind) {
    case LAMBDA_TERM:
        return term_equal(lhs->as.lambda.arg, rhs->as.lambda.arg) && term_equal(lhs->as.lambda.body, rhs->as.lambda.body);
    case VARIABLE_TERM:
        return lhs->finalized ? lhs->as.variable_finalized.i == rhs->as.variable_finalized.i : lhs->as.variable.c == rhs->as.variable.c;
    case APPLICATION_TERM:
        return term_equal(lhs->as.application.lhs, rhs->as.application.lhs) && term_equal(lhs->as.application.rhs, lhs->as.application.rhs);
    }

    assert(0 && "infallible");
    return false;
}

// index table is a linked list so different branches
// of the tree wont clash
struct IndexTable {
    char c;
    uint32_t index;
    struct IndexTable *next; /* nullable */
};

static void _finalize(Term *t, uint32_t depth, struct IndexTable *idx_table);
static uint32_t _get_term_depth(Term *t);

void term_finalize(Term *t)
{
   int32_t depth = _get_term_depth(t); 

   _finalize(t, depth, NULL);
}

static void _finalize(Term *t, uint32_t depth, struct IndexTable *idx_table)
{
    switch (t->kind) {
    case LAMBDA_TERM: {
        char ident = t->as.lambda.arg->as.variable.c;
        t->as.lambda.arg->as.variable_finalized.i = depth;
        t->as.lambda.arg->finalized = true;
        struct IndexTable idx_table_ = (struct IndexTable) {
            .c = ident,
            .index = depth,
            .next = idx_table,
        };
        t->finalized = true;
        _finalize(t->as.lambda.body, depth - 1, &idx_table_);
    } break;
    case APPLICATION_TERM: {
        _finalize(t->as.application.lhs, depth, idx_table);
        _finalize(t->as.application.rhs, depth, idx_table);
        t->finalized = true;
    } break;
    case VARIABLE_TERM: {
        char c = t->as.variable.c;
        
        int32_t found_index = 0;
        while (idx_table != NULL) {
            if (idx_table->c == c) {
                found_index = idx_table->index;
                break;
            } else idx_table = idx_table->next;
        }

        t->as.variable_finalized.i = found_index;
        t->finalized = true;
    } break;
    }
}

static uint32_t _get_term_depth(Term *t)
{
    switch (t->kind) {
    case LAMBDA_TERM: {
        return 1 + _get_term_depth(t->as.lambda.body);
    } break;
    case VARIABLE_TERM: {
        return 0;
    } break;
    case APPLICATION_TERM: {
        uint32_t lhs_depth = _get_term_depth(t->as.application.lhs);
        uint32_t rhs_depth = _get_term_depth(t->as.application.rhs);
        return (lhs_depth > rhs_depth) ? lhs_depth : rhs_depth;
    } break;
    }

    assert(0 && "infallible");
    return 0;
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
        if (t->finalized) 
            printf("%d", t->as.variable_finalized.i);
        else
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

