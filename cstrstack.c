#include <stdbool.h>
#include <stdalign.h>

#include "util.h"
#include "cstrstack.h"

void *cssnew()
{
    return alloc(char*);
}
// return size(by char)
static int fitTo(int size, int w)
{
    int n = size / w;
    if (size % w)
        n++;
    return n*w;
}
cstrstack csspush(cstrstack st, char *str)
{
    // init if zeroed
    char *cur = *st;
    char *begin = (char *)(st + 1);
    if (cur == 0)
        cur = begin;

    ptrdiff_t len = cur - begin;
    int slen = strlen(str);
    int w = sizeof(char *); // align with char*
    int expandsize = fitTo(slen + 1, w) + w;
    st = realloc(st, len + expandsize);
    memcpy(cur, str, slen + 1);
    *st = cur + expandsize;                 // cur to next slot
    *(char **)(cur + expandsize - w) = cur; // tail to previous
    return st;
}
bool cssempty(cstrstack st)
{
    char *cur = *st;
    return cur == NULL || cur == (char *)st + sizeof(char*);
}
char *csspop(cstrstack st)
{
    if (cssempty(st))
        error("pop from empty stack");
    char *cur = *st;
    *st = *(char**)(cur-sizeof(char*)); // make cur point to previous
    return *st;
}