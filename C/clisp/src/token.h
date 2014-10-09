#include <stdio.h>
#include <stdlib.h>

struct cursor
{
    int       line_number
    int       line_offset
};

static const nocursor = {-1, -1}
