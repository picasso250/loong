#include "slice.h"
str *newStr()
{
    return newslice(char, 0, 0);
}
void strExtendCstr(str *st, char *s)
{
    setArrayCap(st->array, st->len + strlen(s));
    memcpy(st->array->base + st->offset + st->len, s, strlen(s));
    st->len += strlen(s);
}
void strExtend(str *st, str *s)
{
    setArrayCap(st->array, st->len + s->len);
    memcpy(st->array->base + st->offset + st->len, s->array->base, s->len);
    st->len += s->len;
}
char *cstr(str *s)
{
    setArrayCap(s->array, s->len + 1);
    s->array->base[s->len] = '\0';
    return s->array->base;
}