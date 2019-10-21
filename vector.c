#include <stdarg.h>
#include <stdlib.h>

#include "Vector.h"
#include "util.h"

#define enlargeIncrement 4
#define enlargeFactor 1.75

Array *_makeArray(int len, int elemSize)
{
    Array *a = alloc(Array);
    a->elemSize = elemSize;
    if (len > 0)
    {
        a->cap = len;
        a->base = calloc(len, elemSize);
    }
    return a;
}

Vector *_makeVectorCap(elemSize, len, cap)
{
    if (len < 0)
        error("len can not be negative");
    if (cap < 0)
        error("cap can not be negative");
    if (len < cap)
        error("len < cap %d < %d", len, cap);
    Array *a = _makeArray(cap, elemSize);
    P(a);
    return _makeVector(a, len, elemSize);
}
Vector *_makeVector(Array *a, int len, int elemSize)
{
    Vector *v = alloc(Vector);
    v->base = a;
    v->len = len;
    v->elemSize = elemSize;
    if (len > 0)
    {
        v->start = a->base;
    }
    return v;
}


static void setArrayCap(Array *a, int newCap)
{
    a->base = realloc(a->base, a->elemSize * newCap);
    checkMem(a->base);
    a->cap = newCap;
}

int lenVector(Vector *v)
{
    return v->len;
}
int cap(Vector *v)
{
    return v->array->cap;
}

Vector *vectorSlice(Vector *v, int start, int end)
{
    if (start > end)
        error("slice start greater than end %d < %d", start, end);
    Vector *vec = alloc(Vector);
    P(vec);
    vec->array = v->array;
    vec->start = v->start+start * v->elemSize;
    vec->len = end - start;
    return vec;
}
Vector *VectorCopy(Vector *v)
{
    Vector *vv = alloc(Vector);
    memcpy(vv, v, sizeof(Vector));
    return vv;
}

int _push(Vector *v, void *p, int elemSize)
{
    if (elemSize != v->elemSize)
        error("not same type, got %d, need %d", elemSize, v->elemSize);
    maybeEnlarge(v, elemSize);
    memcpy(v->start + v->len++ * elemSize, p, elemSize);
    return v->len;
}
static int nextCapacity(int nowCapacity)
{
    return (int)((nowCapacity + enlargeIncrement) * enlargeFactor);
}
void maybeEnlarge(Vector *v, int elemSize)
{
    if (v->array->cap == 0)
        setArrayCap(nextCapacity(0));
    else if (v->start + v->len == v->array->base + v->array->cap)
        setArrayCap(nextCapacity(v->array->cap));
}


int _pop(Vector *v, void *p, int elemSize)
{
    if (v->len == 0)
        error("pop when length is 0");
    memcpy(p, v->start + --v->len * elemSize, elemSize);
    return v->len;
}
void _VectorGet(Vector *v, int i, void *p, int elemSize)
{
    if (i < 0)
        error("index negative");
    if (i >= v->len)
        error("index out of range");
    memcpy(v->start + i * elemSize, p, elemSize);
}
void _VectorSet(Vector *v, int i, void *p, int elemSize)
{
    if (i < 0)
        error("index negative");
    if (i >= v->len)
        error("index out of range");
    memcpy(p, v->start + i * elemSize, elemSize);
}
