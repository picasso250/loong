#include <stdarg.h>
#include <stdlib.h>

#include "slice.h"
// #include "hash.h"
#include "type.h"
#include "util.h"
#include "gc.h"

#define enlargeIncrement 4
#define enlargeFactor 1.75

static void _maybeEnlarge(slice *v, int elemSize);
static int _nextCapacity(int nowCapacity);

array *_initarray(array *arr, char *typeStr, int len, int elemSize)
{
    _zero(arr);
    arr->elemSize = elemSize;
    arr->type = typeStr;
    arr->cap = len;
    if (len > 0)
    {
        arr->base = calloc(len, elemSize);
        checkMem(arr->base);
    }
    return arr;
}

slice *_initslice(slice *v, char *typeStr, int elemSize, int len, int cap)
{
    if (len < 0)
        error("len can not be negative");
    if (cap < 0)
        error("cap can not be negative");
    if (len < cap)
        error("len < cap %d < %d", len, cap);

    _zero(v);
    v->array = _initarray(alloc(array), typeStr, len, elemSize);
    P(v->array);
    v->offset = 0;
    v->len = len;
    // setCustomFlag(v, isPrimitive(typeStr));
    return v;
}

// return to be inserted
void *_push(char *type, slice *v, int elemSize)
{
    if (strcmp(type, v->array->type) != 0)
        error("not same type %s %s", type, v->array->type);
    if (elemSize != v->array->elemSize)
        error("not same type, got %d, need %d", elemSize, v->array->elemSize);
    _maybeEnlarge(v, elemSize);
    return v->array->base + (v->offset + v->len++) * elemSize;
}

static void _setArrayCap(array *a, int newCap)
{
    a->base = realloc(a->base, a->elemSize * newCap);
    checkMem(a->base);
    a->cap = newCap;
}

static slice *_sl(slice *vec, slice *v, int start, int end)
{
    if (start < 0)
        error("start can not be negative");
    if (start > end)
        error("slice start greater than end %d < %d", start, end);
    vec->array = v->array;
    P(v->array);
    vec->offset = v->offset + start;
    vec->len = end - start;
    return vec;
}
slice sl(slice *v, int start, int end)
{
    slice vec;
    _Zero(vec);
    return *_sl(&vec, v, start, end);
}
slice *slp(slice *v, int start, int end)
{
    slice *vec = alloc(slice);
    return _sl(vec, v, start, end);
}

int _pop(slice *v, void *p, int elemSize)
{
    if (v->len == 0)
        error("pop when length is 0");
    v->len--;
    memcpy(p, v->array->base + (v->offset + v->len) * elemSize, elemSize);
    return v->len;
}
void *_sliceget_(slice *v, int i, int elemSize)
{
    if (i < 0)
        error("index negative");
    if (i >= v->len)
        error("index out of range");
    if (elemSize != v->array->elemSize)
        error("elemSize %d assign to %d", v->array->elemSize, elemSize);
    return v->array->base + (v->offset + i) * elemSize;
}
void *_setslice_(char *type, slice *v, int i, int elemSize)
{
    if (strcmp(type, v->array->type) != 0)
        error("not same type %s %s", type, v->array->type);
    if (i < 0)
        error("index negative");
    if (i >= v->len)
        error("index out of range");
    return v->array->base + (v->offset + i) * elemSize;
}

void _copy(slice *dst, int i, int ii, slice *src, int k, int kk, int size1, int size2)
{
    if (size1 != size2)
        error("size not equal %d!=%d when copy", size1, size2);
    if (strcmp(dst->array->type, src->array->type) != 0)
        error("type not equal %s %s", src->array->type, dst->array->type);
    if (dst == NULL)
        error("dst is NULL");
    if (src == NULL)
        error("src is NULL");
    if (i > ii)
        error("index not possible");
    if (k > kk)
        error("index not possible");
    if (i == ii || k == kk)
        return;
    int d1 = ii - i;
    int d2 = kk - k;
    int d = min(d1, d2);
    char *ds = dst->array->base + (dst->offset + i) * size1;
    char *sr = src->array->base + (src->offset + k) * size2;
    memmove(ds, sr, size1 * d);
}

array *arrayfinal(array *a)
{
    if (a)
    {
        if (a->base )
        {
            free(a->base);
        }
        a->base = NULL;
    }
    return a;
}
slice *slicefinal(slice *s)
{
    if (s)
    {
        if (s->array == NULL)
            error("s->array is NULL");
        V(s->array);
    }
    return s;
}

// ==== private ====
static void _maybeEnlarge(slice *v, int elemSize)
{
    if (v->array->cap == 0)
    {
        _setArrayCap(v->array, _nextCapacity(0));
    }
    else if (v->offset + v->len  ==  v->array->cap )
    {
        _setArrayCap(v->array, _nextCapacity(v->array->cap));
    }
}

static int _nextCapacity(int nowCapacity)
{
    return (int)((nowCapacity + enlargeIncrement) * enlargeFactor);
}
