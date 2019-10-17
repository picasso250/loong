#include <stdbool.h>

#include "util.h"
#include "cval.h"

int type(cval *v)
{
    return (v->flag & 0xEFFF0000) >> 16;
}
int _refCount(cval *v)
{
    return (v->flag & 0xFFFF);
}
int _is_sweep(cval *v)
{
    return (v->flag & 0x80000000);
}

cval *_make_array_(int len, int elemSize)
{
    cval *cval = alloc(cval);
    _set_type(cval, t_array);
    _zero(&(cval->v_array));
    cval->v_array.len = len;
    cval->v_array.base = calloc(len, sizeof(cval) * elemSize);
    checkMem(cval->v_array.base);
    cval->v_array.elemSize = elemSize;
    return cval;
}

cval *_make_slice_(cval *arr, int len)
{
    cval *cval = alloc(cval);
    _set_type(cval, t_slice);
    _zero(&(cval->v_slice));
    cval->v_slice.len = len;
    cval->v_slice.base = arr->v_array.base;
    return cval;
}
cval *_make_slice__(cval *arr, int len,int cap)
{
    if (len < 0)
        error("len can not be negative");
    if (cap < 0)
        error("cap can not be negative");
    if (len < cap)
        error("len < cap %d < %d", len, cap);
    return _make_slice_(arr, len);
}

#define _define_make_type_primary(t) \
    cval *_make_##t(##t v)           \
    {                                \
        cval *val = alloc(cval);     \
        _set_type(val, t_##t);       \
        val->v_##t = v;              \
        return val;                  \
    }
_define_make_type_primary(int)
    _define_make_type_primary(float)
        _define_make_type_primary(byte)
            _define_make_type_primary(bool)
