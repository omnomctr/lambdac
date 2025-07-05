#ifndef DATA_HEADER__
#define DATA_HEADER__ 

#include "arena.h"

typedef struct Term Term;
struct Term {
    enum { 
        LAMBDA_TERM,
        VARIABLE_TERM,
        APPLICATION_TERM,
    } kind;

    union {
        struct {
            Term *arg;
            Term *body;
        } lambda;
        struct {
            char c;
        } variable;
        struct {
            Term *lhs;
            Term *rhs;
        } application;
    } as;
};

Term *lambda_new(Arena *a, Term *arg, Term *body);
Term *variable_new(Arena *a, char c);
Term *application_new(Arena *a, Term *lhs, Term *rhs);

void term_print(Term *t);

#endif
