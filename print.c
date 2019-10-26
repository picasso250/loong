#include "buildin.h"
#include "print.h"

static void _str(str *st, const char *type, void *base, int offset);
str *arrayStr(array *a)
{
    str *st = newStr();
    strExtendCstr(st, "[ ");
    char buf[22]; // big enough for int64
    str *s0 = NULL;
    for (int i = 0; i < len(a); i++)
    {
        _str(st, a->type, a->base, i);
        strExtendCstr(st, ", ");
    }
    free(slicefinal(s0));
    strExtendCstr(st, "]");
    return st;
}

str *sliceStr(slice *a)
{
    str *st = newStr();
    strExtendCstr(st, "[ ");
    for (int i = 0; i < len(a); i++)
    {
        _str(st, a->array->type, a->array->base, a->offset + i);
        strExtendCstr(st, ", ");
    }
    strExtendCstr(st, "]");
    return st;
}
str *strStr(str *a)
{
    return a;
}

str *mapStr(map *m)
{
    str *st = newStr();
    strExtendCstr(st, "[ ");
    char buf[22]; // big enough for int64
    str *s0 = NULL;
    for (int _i_ = 0; _i_ < m->factor; _i_++)
    {
        for (mapentry *_pme_ = m->base[_i_]; _pme_; _pme_ = _pme_->next)
        {
            char *k = _pme_->key;
            strExtendCstr(st, k);
            strExtendCstr(st, ": ");
            _str(st, m->type, _pme_->value, 0);
            strExtendCstr(st, ", ");
        }
    }
    free(slicefinal(s0));
    strExtendCstr(st, "]");
    return st;
}
static void _str(str *st, const char *type, void *base, int offset)
{
    char buf[22]; // big enough for int64
    str *s0 = NULL;
    if (strcmp(type, "array") == 0)
    {
        strExtend(st, s0 = arrayStr((array *)base + offset));
    }
    else if (strcmp(type, "slice") == 0)
    {
        strExtend(st, s0 = sliceStr((slice *)base + offset));
    }
    else if (strcmp(type, "str") == 0)
    {
        strExtendCstr(st, "\"");
        strExtend(st, (str *)base + offset);
        strExtendCstr(st, "\"");
    }
    else if (strcmp(type, "map") == 0)
    {
        strExtend(st, s0 = mapStr((map *)base + offset));
    }
    else if (strcmp(type, "int") == 0)
    {
        sprintf(buf, "%d", *((int *)base + offset));
        strExtendCstr(st, buf);
    }
    else if (strcmp(type, "float") == 0)
    {
        sprintf(buf, "%f", *((float *)base + offset));
        strExtendCstr(st, buf);
    }
    else if (strcmp(type, "char") == 0)
    {
        sprintf(buf, "%c", *((char *)base + offset));
        strExtendCstr(st, buf);
    }
    else if (strcmp(type, "char *") == 0)
    {
        strExtendCstr(st, "\"");
        strExtendCstr(st, *((char **)base + offset));
        strExtendCstr(st, "\"");
    }
    else
    {
        error("unknown type: %s", type);
    }
    free(slicefinal(s0));
}
