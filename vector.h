#ifndef VECTOR_H
#define VECTOR_H

#include <stdbool.h>

#include "gc.h"

typedef struct array
{
    GCFlag flag;
    void *base;
    int cap;
    int elemSize;
} array;
typedef struct vector
{
    GCFlag flag;
    array *array;
    void *start;
    int len;
    int elemSize;
} vector;

#define makeArray(t, len) _makeArray(len, sizeof(t))

#define makeVector(t, len) makeVectorCap(t, len, len)
#define makeVectorCap(t, len, cap) _makeVectorCap(sizeof(t), len, cap)

#define push(v, a)      \
    do                  \
    {                   \
        void *p = &(a); \
        _push(v, a, p); \
    } while (0)
#define push_(v, a, p) _Generic(a,                         \
                                primitiveDef( _push(v, p, sizeof(*p))) \
                                  float                    \
                                : _push(v, p, sizeof(*p)), \
                                  byte                     \
                                : _push(v, p, sizeof(*p)), \
                                  bool                     \
                                : _push(v, p, sizeof(*p)), \
                                  default                  \
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
#define primitiveDef(...) _primitiveDef(int, __VA_ARGS__),   \
                          _primitiveDef(float, __VA_ARGS__), \
                          _primitiveDef(byte, __VA_ARGS__),  \
                          _primitiveDef(bool, __VA_ARGS__),
#define VectorGet(v, i, a)               \
    do                                   \
    {                                    \
        void *p = &(a);                  \
        _VectorGet(v, i, p, sizeof(*p)); \
    } while (0)
#define VectorSet(v, i, a)               \
    do                                   \
    {                                    \
        void *p = &(a);                  \
        P(a);                            \
        _VectorSet(v, i, p, sizeof(*p)); \
    } while (0)

int lenVector(vector *v);
int cap(vector *v);
vector *VectorCopy(vector *v);
vector *VectorSlice(vector *v, int start, int end);

#endif