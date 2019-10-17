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

#define make(t, ...) _Generic(t,                          \
                              int                         \
                              : _make_int(__VA_ARGS__),   \
                                float                     \
                              : _make_float(__VA_ARGS__), \
                                byte                      \
                              : _make_byte(__VA_ARGS__),  \
                                bool                      \
                              : _make_bool(__VA_ARGS__),  \
                                array                     \
                              : _make_array(__VA_ARGS__), \
                                slice                     \
                              : _make_slice(__VA_ARGS__), \
                                map                       \
                              : _make_map(__VA_ARGS__), )

#define _make_array(t, len) _make_array_((len), sizeof(t))
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
#define _VEC_GET_MACRO(_1, _2, _3, NAME, ...) NAME
#define _make_slice(...)                                      \
    _VEC_GET_MACRO(__VA_ARGS__, _make_slice_3, _make_slice_2) \
    (__VA_ARGS__)
#define _make_slice_3(t, len, cap)                \
    do                                            \
    {                                             \
        if (len < 0)                              \
            error("len can not be negative");     \
        if (cap < 0)                              \
            error("cap can not be negative");     \
        if (len < cap)                            \
            error("len < cap %d < %d", len, cap); \
        _make_slice_(_make_array_(t, cap), len)   \
    } while (0)
#define _make_slice_2(t, len) _make_slice_3(t, len, len)

cval *_make_slice_(cval *arr, int len)
{
    cval *cval = alloc(cval);
    _set_type(cval, t_slice);
    _zero(&(cval->v_slice));
    cval->v_slice.len = len;
    cval->v_slice.base = arr->v_array.base;
    return cval;
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
