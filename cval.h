#ifndef CVAL_H
#define CVAL_H

#define t_int 1
#define t_float 2
#define t_byte 3
#define t_bool 4
#define t_array 5
#define t_slice 6
#define t_map 7

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
typedef struct map
{
    int len;
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

#endif