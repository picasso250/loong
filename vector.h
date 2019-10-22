#ifndef _SLICE_H_
#define _SLICE_H_

#include <stdbool.h>

#include "gc.h"

typedef struct array
{
    GCFlag flag;
    char *type;
    int cap;
    int elemSize;
    char *base;
} array;
typedef struct slice
{
    GCFlag flag;
    array *array;
    int len;
    char *start;
} slice;

/*
 * there are two ways to check type, #t and _Generic
 * former is first try, then _Generic
 * types are checked dynamically
 */
#define _sizeof(t) (isPrimitive(#t) ? sizeof(t) : sizeof(t *))

#define _makearray(t, len) _makearray(#t, alloc(array), len, _sizeof(t))

#define _makeslice(t, len) _makesliceCap(makearray(t, cap), #t, _sizeof(t), len, cap)

#define push(v, a) _push(v, a)

#define sliceSet(v, i, a)                \
    do                                    \
    {                                     \
        void *p = &(a);                   \
        P(*p);                            \
        _sliceSet_(v, i, p, sizeof(*p)); \
    } while (0)
#define _sliceSet_(v, i, a, p) _Generic(a, primitiveDef(_sliceSet(v, i, p, sizeof(a))), \
                                         default                                          \
                                         : _sliceSet(v, i, a, P(*p)))
int lenslice(slice *v);
int cap(slice *v);
slice *sliceCopy(slice *v);
slice *sliceSlice(slice *v, int start, int end);

#define pop(t, v) _slicepop##t(#t, v, sizeof(v->base[0]), sizeof(t))

#define defslicePop(t)                                                       \
    t _slicepop##t(char *typeStr, slice##t *v, int elemSize, int elemSizeT) \
    {                                                                         \
        if (strcmp(typeStr, #t) != 0)                                         \
            error("type error %d %d when pop", typeStr, #t);                  \
        t aa;                                                                 \
        _slicepop((slice *)v, &aa, elemSize, elemSizeT);                    \
        return aa;                                                            \
    }
#define pop(t) _Generic((t)->start[0], \
    int:_vectorpopint(v,sizeof(int)), \
    unsigned:_vectorpopunsigned(v,sizeof(unsigned)), \
    short:_vectorpopshort(v,sizeof(short)), \
    long:_vectorpoplong(v,sizeof(long)), \
    float:_vectorpopfloat(v,sizeof(float)), \
    double:_vectorpopdouble(v,sizeof(double)), \
    char:_vectorpopchar(v,sizeof(char)), \
    byte:_vectorpopbyte(v,sizeof(byte)), \
    bool:_vectorpopbool(v,sizeof(bool)), \
    default:_vectorpopdefault(v,sizeof(char*)))

    _defsliceGet(int)
_defsliceGet(unsigned)
_defsliceGet(short)
_defsliceGet(long)
_defsliceGet(float)
_defsliceGet(double)
_defsliceGet(char)
_defsliceGet(byte)
_defsliceGet(bool)
_defsliceGet(default)
#endif
