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
    cval *array;
    struct cval *base;
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
        array v_array; // array
        slice v_slice; // slice
        map v_map;     // map
    };
} cval;
// flag 32 bit
// 32              31 --- 16        15 --- 0
//  ^--sweep-flag   ^------^--type   ^-----^--ref-count

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
                              : _make_map(__VA_ARGS__),   \
                                str                       \
                              : _make_str(__VA_ARGS__))

#define _make_array(t, len) _make_array_((len), sizeof(t))


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

cval* _make_str(char*s){
    int len = strlen(s);
     _make_slice_3(char,len,len+1);
}

#define _VEC_GET_MACRO_1(_1, NAME, ...) NAME
#define _make_map(...)                                      \
    _VEC_GET_MACRO_1(__VA_ARGS__, _make_map_1, _make_map_0) \
    (__VA_ARGS__)
#define _make_map_1(len) _make_map_(len)
#define _make_map_0() _make_map_1(0)

#endif