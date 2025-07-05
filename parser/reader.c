#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include "reader.h"

int readchar(struct Reader *r)
{
    return r->_vtable.readchar(r);
}

int _zstr_readchar(struct Reader *self) 
{
    union { struct Reader *orig; ZStrReader *self; } u;
    u.orig = self;

    assert(!u.self->at_eof);
    char c = u.self->ptr[0];

    if (c == '\0') {
        u.self->at_eof = true;
        return EOF;
    } else {
        u.self->ptr++;
        return (int)c;
    }
}

ZStrReader zstrreader_new(char * const s)
{
    return (ZStrReader) {
        ._vtable = (struct ReaderVtable) {
            .readchar = _zstr_readchar,
        },
        .ptr = s,
        .at_eof = false,
    };
}

inline struct Reader *zstrreader_to_reader(ZStrReader *r)
{
    // union bc type punning is ub otherwise
    union { ZStrReader *orig; struct Reader *dest; } u;
    u.orig = r;
    return u.dest;
}

int _file_readchar(struct Reader *self) 
{
    union { struct Reader *orig; FileReader *self; } u;
    u.orig = self;

    return fgetc(u.self->f);
}

FileReader filereader_new(FILE *f) 
{
    return (FileReader) {
        ._vtable = (struct ReaderVtable) {
            .readchar = _file_readchar,
        },
        .f = f
    };
}

inline struct Reader *filereader_to_reader(FileReader *r)
{
    union { FileReader *orig; struct Reader *dest; } u;
    u.orig = r;
    return u.dest;
}


