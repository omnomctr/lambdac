#ifndef DATA_HEADER__
#define DATA_HEADER__ 

#include "arena.h"
#include <stdbool.h>
#include <stdint.h>

typedef struct Term Term;
struct Term {
    enum { 
        LAMBDA_TERM,
        VARIABLE_TERM,
        APPLICATION_TERM,
    } kind;

    bool finalized;

    union {
        struct {
            Term *arg;
            Term *body;
        } lambda;
        struct {
            char c;
        } variable;
        struct {
            uint32_t i;
        } variable_finalized;
        struct {
            Term *lhs;
            Term *rhs;
        } application;
    } as;
};

Term *lambda_new(Arena *a, Term *arg, Term *body);
Term *variable_new(Arena *a, char c);
Term *application_new(Arena *a, Term *lhs, Term *rhs);

// finalize w/ de bruijn indexing
void term_finalize(Term *t);

void term_print(Term *t);
bool term_equal(Term *lhs, Term *rhs);

#endif
