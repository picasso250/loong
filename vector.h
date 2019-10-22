#ifndef VECTOR_H
#define VECTOR_H

#include <stdbool.h>

#include "gc.h"

typedef struct array
{
    GCFlag flag;
    char *type;
    void *base;
    int cap;
    int elemSize;
} array;
typedef struct vector
{
    GCFlag flag;
    char *type;
    array *array;
    void *start;
    int len;
    int elemSize;
} vector;

#define makeArray(t, len) _makeArray_(#t, len, sizeofType(t))
#define sizeofType(t) isPrimitive(typeStr) ? sizeof(t) : sizeof(t *)

#define makeVector(t, len) makeVectorCap(t, len, len)
#define makeVectorCap(t, len, cap) _makeVectorCap(makeArray(t,cap),#t,sizeofType(t), len, cap)

#define push(v, a)      \
    do                  \
    {                   \
        void *p = &(a); \
        push_(v, a, p); \
    } while (0)
#define push_(v, a, p) _Generic(a,                                     \
                                primitiveDef(_push(v, p, sizeof(*p))), \
                                default                                \
                                : _push(v, P(*p), sizeof(*p)))
#define pop(v, a)               \
    do                          \
    {                           \
        void *p = &(a);         \
        _pop(v, p, sizeof(*p)); \
    } while (0)
#define _primitiveDef(t, ...) \
    t:                        \
    __VA_ARGS__
#define primitiveDef(...) _primitiveDef(int, __VA_ARGS__), \
                          _primitiveDef(float, __VA_ARGS__), \
                          _primitiveDef(bool, __VA_ARGS__), \
                          _primitiveDef(byte, __VA_ARGS__)
#define VectorGet(v, i, a)               \
    do                                   \
    {                                    \
        void *p = &(a);                  \
        _VectorGet(v, i, p, sizeof(*p)); \
    } while (0)
#define VectorSet(v, i, a)                \
    do                                    \
    {                                     \
        void *p = &(a);                   \
        P(*p);                            \
        _VectorSet_(v, i, p, sizeof(*p)); \
    } while (0)
#define _VectorSet_(v, i, a, p) _Generic(a, primitiveDef(_VectorSet(v, i, p, sizeof(a))), \
                                         default                                          \
                                         : _VectorSet(v, i, a, P(*p)))
int lenVector(vector *v);
int cap(vector *v);
vector *VectorCopy(vector *v);
vector *VectorSlice(vector *v, int start, int end);

#endif
