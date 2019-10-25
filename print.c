#include "buildin.h"
#include "print.h"

str *arrayStr(array *a)
{
    str *st = newStr();
    strExtendCstr(st, "[ ");
    char buf[22]; // big enough for int64
    str *s0 = NULL;
    for (int i = 0; i < len(a); i++)
    {
        if (strcmp(a->type, "array") == 0)
        {
            strExtend(st, s0 = arrayStr((array *)a->base + i));
        }
        else if (strcmp(a->type, "slice") == 0)
        {
            strExtend(st, s0 = sliceStr((slice *)a->base + i));
        }
        else if (strcmp(a->type, "map") == 0)
        {
            strExtend(st, s0 = mapStr((map *)a->base + i));
        }
        else if (strcmp(a->type, "int") == 0)
        {
            sprintf(buf, "%d", *((int *)a->base + i));
            strExtendCstr(st, buf);
        }
        else if (strcmp(a->type, "float") == 0)
        {
            sprintf(buf, "%f", *((float *)a->base + i));
            strExtendCstr(st, buf);
        }
        else if (strcmp(a->type, "char") == 0)
        {
            sprintf(buf, "%c", *((char *)a->base + i));
            strExtendCstr(st, buf);
        }
        else if (strcmp(a->type, "char*") == 0)
        {
            strExtendCstr(st, *((char **)a->base + i));
        }
        else
        {
            error("unknown a->type: %s", a->type);
        }
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
    char buf[22]; // big enough for int64
    str *s0 = NULL;
    for (int i = 0; i < len(a); i++)
    {
        if (strcmp(a->array->type, "array") == 0)
        {
            strExtend(st, s0 = arrayStr((array *)a->array->base + a->offset + i));
        }
        else if (strcmp(a->array->type, "slice") == 0)
        {
            strExtend(st, s0 = sliceStr((slice *)a->array->base + a->offset + i));
        }
        else if (strcmp(a->array->type, "map") == 0)
        {
            strExtend(st, s0 = mapStr((map *)a->array->base + a->offset + i));
        }
        else if (strcmp(a->array->type, "int") == 0)
        {
            sprintf(buf, "%d", *((int *)a->array->base + a->offset + i));
            strExtendCstr(st, buf);
        }
        else if (strcmp(a->array->type, "float") == 0)
        {
            sprintf(buf, "%f", *((float *)a->array->base + a->offset + i));
            strExtendCstr(st, buf);
        }
        else if (strcmp(a->array->type, "char") == 0)
        {
            sprintf(buf, "%c", *((char *)a->array->base + a->offset + i));
            strExtendCstr(st, buf);
        }
        else if (strcmp(a->array->type, "char *") == 0)
        {
            strExtendCstr(st, "\"");
            strExtendCstr(st, *((char **)a->array->base + a->offset + i));
            strExtendCstr(st, "\"");
        }
        else
        {
            error("unknown a->array->type: %s", a->array->type);
        }
        strExtendCstr(st, ", ");
    }
    free(slicefinal(s0));
    strExtendCstr(st, "]");
    return st;
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
            if (strcmp(m->type, "array") == 0)
            {
                strExtend(st, s0 = arrayStr((array *)_pme_->value));
            }
            else if (strcmp(m->type, "slice") == 0)
            {
                strExtend(st, s0 = sliceStr((slice *)_pme_->value));
            }
            else if (strcmp(m->type, "map") == 0)
            {
                strExtend(st, s0 = mapStr((map *)_pme_->value));
            }
            else if (strcmp(m->type, "int") == 0)
            {
                sprintf(buf, "%d", *((int *)_pme_->value));
                strExtendCstr(st, buf);
            }
            else if (strcmp(m->type, "float") == 0)
            {
                sprintf(buf, "%f", *((float *)_pme_->value));
                strExtendCstr(st, buf);
            }
            else if (strcmp(m->type, "char") == 0)
            {
                sprintf(buf, "%c", *((char *)_pme_->value));
                strExtendCstr(st, buf);
            }
            else if (strcmp(m->type, "char *") == 0)
            {
                strExtendCstr(st, *((char **)_pme_->value));
            }
            else
            {
                error("unknown m->type: %s", m->type);
            }
            strExtendCstr(st, ", ");
        }
    }
    free(slicefinal(s0));
    strExtendCstr(st, "]");
    return st;
}