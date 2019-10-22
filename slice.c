#include <stdarg.h>
#include <stdlib.h>

#include "vector.h"
#include "hash.h"
#include "type.h"
#include "util.h"

#define enlargeIncrement 4
#define enlargeFactor 1.75

array *_makeArray_(char *typeStr, void *arr, int len, int elemSize)
{
    array *a = arr;
    a->elemSize = elemSize;
    a->type = typeStr;
    a->cap = len;
    if (len > 0)
    {
        a->base = calloc(len, elemSize);
        checkMem(a->base);
    }
    return a;
}

vector *_makeVectorCap(array *a, char *typeStr, int elemSize, int len, int cap)
{
    if (len < 0)
        error("len can not be negative");
    if (cap < 0)
        error("cap can not be negative");
    if (len < cap)
        error("len < cap %d < %d", len, cap);
    P(a);
    return _makeVector(a, typeStr, len, elemSize);
}
vector *_makeVector(array *a, char *typeStr, int len, int elemSize)
{
    vector *v = alloc(vector);
    v->start = a->base;
    v->len = len;
    v->elemSize = elemSize;
    v->type = a->type;
    setCustomFlag(v, isPrimitive(typeStr));
    v->start = a->base;
    P(v);
    return v;
}

static void setArrayCap(array *a, int newCap)
{
    a->base = realloc(a->base, a->elemSize * newCap);
    checkMem(a->base);
    a->cap = newCap;
}

int lenVector(vector *v)
{
    return v->len;
}
int cap(vector *v)
{
    return v->array->cap;
}

vector *_sliceslicedefault(vector *v, int start, int end)
{
    if (start > end)
        error("slice start greater than end %d < %d", start, end);
    vector *vec = alloc(vector);
    P(vec);
    vec->array = v->array;
    vec->start = v->start + start * v->array->elemSize;
    vec->len = end - start;
    return vec;
}
vector *VectorCopy(vector *v)
{
    vector *vv = alloc(vector);
    memcpy(vv, v, sizeof(vector));
    vv->flag = 0;
    P(vv);
    return vv;
}

int _push(vector *v, void *p, int elemSize)
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
void maybeEnlarge(vector *v, int elemSize)
{
    if (v->array->cap == 0)
        setArrayCap(nextCapacity(0));
    else if (v->start + v->len == v->array->base + v->array->cap)
        setArrayCap(nextCapacity(v->array->cap));
}

int _vectorpop(vector *v, void *p, int elemSize, int elemSizeT)
{
    if (v->len == 0)
        error("pop when length is 0");
    memcpy(p, v->start + --v->len * elemSize, elemSize);
    return v->len;
}
void _VectorGet(vector *v, int i, void *p, int elemSize)
{
    if (i < 0)
        error("index negative");
    if (i >= v->len)
        error("index out of range");
    memcpy(v->start + i * elemSize, p, elemSize);
}
void _VectorSet(vector *v, int i, void *p, int elemSize)
{
    if (i < 0)
        error("index negative");
    if (i >= v->len)
        error("index out of range");
    memcpy(p, v->start + i * elemSize, elemSize);
}
void vectorOnGc(Vector *v)
{
    // free
}