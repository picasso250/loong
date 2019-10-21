#ifndef VECTOR_H
#define VECTOR_H

#include "gc.h"

typedef struct array
{
    GCFlag flag;
    void *base;
    int cap;
    int elemSize;
} Array;
typedef struct vector
{
    GCFlag flag;
    Array *array;
    void *start;
    int len;
    int elemSize;
} Vector;

#define MakeArray(t, len) _makeArray(len, sizeof(t))

#define MakeVector(t, len) MakeVectorCap(t, len, len)
#define MakeVectorCap(t, len, cap) _MakeVectorCap(sizeof(t),len,cap)


#define push(v, e)                                            \
    do                                                        \
    {                                                         \
        Array *arr = v->array;                                 \
        char *p = v->start;                                   \
        char *end = p + arr->elemSize * len(v);               \
        if (end > (char *)arr->data + cap(v) * arr->elemSize) \
            _vectorEnlarge(v, cap(v) + 1);                    \
        v->start[v->len] = e;                                 \
        v->len++;                                             \
    } while (0)

#define _defineVectorGet(t)           \
    t _vectorGet##t(vector *v, int i) \
    {                                 \
        return ((t *)v->start)[i];      \
    }
_defineVectorGet(int)
#define vectorGet(v,i) _Generic(t,int:_vectorGetint) (v,t)
#define vectorSet(v,)


int len(vector *v);
int cap(vector *v);
int copy(vector *dst, vector *src);
vector *vectorSlice(vector *v, int start, int end);

#endif