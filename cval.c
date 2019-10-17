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
cval *_make_slice__(cval *arr, int len, int cap)
{
    if (len < 0)
        error("len can not be negative");
    if (cap < 0)
        error("cap can not be negative");
    if (len < cap)
        error("len < cap %d < %d", len, cap);
    return _make_slice_(arr, len);
}

cval *_make_str(char *s)
{
    int len = strlen(s);

    cval *cval = alloc(cval);
    _set_type(cval, t_array);
    _zero(&(cval->v_array));
    cval->v_array.len = len;
    cval->v_array.base = s;
    cval->v_array.elemSize = 1;
    
    return _make_slice_(cval, len);
}

cval *equal(cval *left, cval *right)
{
    if (left == right)
        return true;
    bool b;
    if (type(left) != type(right))
    {
    }
    else
    {
        b = _equal_type_equal(type(left), left, right);
    }
    return var(b);
}
bool _equal_type_equal(int t, cval *left, cval *right)
{
    switch (t)
    {
    case t_int:
        return left->v_int == right->v_int;
    case t_float:
        return left->v_float == right->v_float;
    case t_byte:
        return left->v_byte == right->v_byte;
    case t_bool:
        return left->v_bool == right->v_bool;
    case t_array:
        return _equal_array(left->v_array, right->v_array);
    case t_slice:
        return _equal_slice(left->v_slice, right->v_slice);
    case t_map:
        error("map can not be compared");
    case t_str:
        return _equal_str(left->v_str, right->v_str);
    default:
        error("unknown type %d\n", t);
    }
}
bool _equal_array(array left, array right)
{
    return left.base == right.base &&
           left.len == right.len &&
           left.elemSize == right.elemSize;
}
bool _equal_slice(slice left, slice right)
{
    if (left.len != right.len)
        return false;
    if (left.array->v_array.elemSize != right.array->v_array.elemSize)
        return false;
    int elemSize = left.array->v_array.elemSize;
    return memcmp(left.base, right.base, left.len * elemSize) == 0;
}
bool _equal_str(str left, str right)
{
    return _equal_slice(left, right);
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
