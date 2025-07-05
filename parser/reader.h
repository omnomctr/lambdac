#ifndef READER_HEADER__
#define READER_HEADER__

#include <stdbool.h>
#include <stdio.h>

struct Reader;
struct ReaderVtable {
    int (*readchar)(struct Reader *self);
};

struct Reader {
    struct ReaderVtable _vtable;
};

int readchar(struct Reader *r);


typedef struct ZStrReader {
    struct ReaderVtable _vtable;
    char *ptr;
    bool at_eof;
} ZStrReader;

ZStrReader zstrreader_new(char * const s);
struct Reader *zstrreader_to_reader(ZStrReader *);

typedef struct FileReader {
    struct ReaderVtable _vtable;
    FILE *f;
} FileReader;


FileReader filereader_new(FILE *f);
struct Reader *filereader_to_reader(FileReader *);

#endif
