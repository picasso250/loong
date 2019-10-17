#ifndef CVAL_H
#define CVAL_H

#include "buildin.h"

#define t_int 1
#define t_float 2
#define t_byte 3
#define t_bool 4
#define t_array 5
#define t_slice 6
#define t_map 7
#define t_str 8

typedef char byte;
typedef struct array
{
    char *base;
    int len;
    int elemSize;
} array;
typedef struct slice
{
    struct cval *array;
    char *base;
    int len;
} slice;
typedef slice str;
typedef struct map
{
    int len;
    int factor;
    struct cval *first;
    struct mapentry **base;
} map;
typedef struct mapentry
{
    struct cval *key;
    struct cval *value;
    struct cval *next;
} mapentry;
typedef struct cval
{
    unsigned flag; // flag && type
    union {
        int v_int;     // int
        float v_float; // float
        char v_byte;   // byte
        bool v_bool;   // bool
        str v_str;     // string
        array v_array; // array
        slice v_slice; // slice
        map v_map;     // map
    };
} cval;
// flag 32 bit
// 32              31 --- 16        15 --- 0
//  ^--sweep-flag   ^------^--type   ^-----^--ref-count

#define var(x) _Generic(x,             \
                        int            \
                        : _make_int,   \
                          float        \
                        : _make_float, \
                          byte         \
                        : _make_byte,  \
                          bool         \
                        : _make_bool,  \
                          char *       \
                        : _make_str)(x)

#define make(t, ...) _make_##t(__VA_ARGS__)

#define _make_array(t, len) _make_array_((len), sizeof(t))

#define _VEC_GET_MACRO(_1, _2, _3, NAME, ...) NAME
#define _make_slice(...)                                      \
    _VEC_GET_MACRO(__VA_ARGS__, _make_slice_3, _make_slice_2) \
    (__VA_ARGS__)
#define _make_slice_3(t, len, cap) _make_slice__(_make_array(t, cap), len, cap)
#define _make_slice_2(t, len) _make_slice__(_make_array(t, len), len, len)

#define _VEC_GET_MACRO_1(_1, NAME, ...) NAME
#define _make_map(...)                                      \
    _VEC_GET_MACRO_1(__VA_ARGS__, _make_map_1, _make_map_0) \
    (__VA_ARGS__)
#define _make_map_1(len) _make_map_(len)
#define _make_map_0() _make_map_(0)

#endif