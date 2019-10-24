#include <stdio.h>

#include "util.h"
void *_alloc_and_zero(int size, char *file, int line, char *t)
{
    void *p = malloc(size);
    if (!p)
    {
        fprintf(stderr, "%s:%d: memory allocation failed for type %s\n", file, line, t);
        abort();
    }

    return memset(p, 0, size);
}
