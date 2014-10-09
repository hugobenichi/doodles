#include <stdio.h>
#include <stdlib.h>

struct line
{
    *byte     line
    int       number
    int       len
};

static const noline = {NULL, -1};

struct reader;

// add file descriptor or something to get bytes from
// like info struct
int reader_init(*struct reader, int max_mem, int max_line);
int reader_clean(*struct reader);

// push reader
// file reader
int reader_from_file(*struct reader, int max_mem, int max_line)

// all idempotent ops
struct line line_at(*struct reader, int line_number);
struct line next_line(*struct reader, *struct line); // first line if null
struct line prev_line(*struct reader, *struct line); // last line if null
