#include <stdbool.h>
#include <stdalign.h>

#include "util.h"

// structure:
// char*cur; elems...
// elem:
// [char]; int len;
typedef char **cstrstack;
void *cssnew()
{
    return _alloc_and_zero(sizeof(int) + sizeof(char *), __FILE__, __LINE__, "cstrstack");
}
void csspush(cstrstack st, char *str)
{
    char *cur = *st;
    char *begin = (char *)(st + 1);
    if (cur == 0)
        *(char **)st = begin;

    ptrdiff_t len = cur - begin;
    int slen = strlen(str); // align
    int wordlen = sizeof(int);
    int blocklen = (slen + 1) / wordlen;
    if ((slen + 1) % wordlen)
        blocklen++;
    st = realloc(st, len + blocklen * wordlen + wordlen);
    memcpy(cur, str, slen + 1);
    *(char *)((int *)cur + blocklen) = cur;
    **st = (char *)((int *)cur + blocklen + 1); // to next slot
}
bool cssempty(cstrstack st)
{
    char *cur = *st;
    return cur == 0 || cur == st + 1;
}
char *csspop(cstrstack st)
{
    if (cssempty(st))
        error("pop from empty stack");
    char *cur = *(char **)(st + 1);
    **st = (int *)cur - 1;
    if (cssempty(st))
        st = realloc(st, sizeof(char *));
}