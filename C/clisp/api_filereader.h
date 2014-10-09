// to read a file or a stream line by line

struct line
{
    *byte     line
    int       number
    int       len
};

static const noline = {NULL, -1};

struct filereader;

// add file descriptor or something to get bytes from
// like file_info struct
int filereader_init(*struct filereader, int max_mem, int max_line);
int filereader_clean(*struct filereader);

// all idempotent ops
struct line line_at(*struct filereader, int line_number);
struct line next_line(*struct filereader, *struct line); // first line if null
struct line prev_line(*struct filereader, *struct line); // last line if null

// to tokenize in words a given filereader given a parse strategy
// given current cursor and file reader,
// get next cursor
// by applying a parsing strategy

struct cursor
{
    int       line_number
    int       line_offset
};

static const nocursor = {-1, -1}

typedef int *parser(*byte, int);

struct cursor
