#include <stdarg.h>
#include <stdlib.h>

#include "slice.h"
#include "hash.h"
#include "type.h"
#include "util.h"
#include "gc.h"

#define enlargeIncrement 4
#define enlargeFactor 1.75

static void _maybeEnlarge(slice *v, int elemSize);
static slice _makeslice_(array *a, char *typeStr, int len, int elemSize);
static int _nextCapacity(int nowCapacity);

array _makearray_(char *typeStr, array *arr, int len, int elemSize)
{
    array a = *arr;
    a.elemSize = elemSize;
    a.type = typeStr;
    a.cap = len;
    if (len > 0)
    {
        a.base = calloc(len, elemSize);
        checkMem(a.base);
    }
    return a;
}

slice _makesliceCap(array a, char *typeStr, int elemSize, int len, int cap)
{
    if (len < 0)
        error("len can not be negative");
    if (cap < 0)
        error("cap can not be negative");
    if (len < cap)
        error("len < cap %d < %d", len, cap);
    array *arr = alloc(array);
    *arr = a;
    P(arr);
    return _makeslice_(arr, typeStr, len, elemSize);
}

static void _setArrayCap(array *a, int newCap)
{
    a->base = realloc(a->base, a->elemSize * newCap);
    checkMem(a->base);
    a->cap = newCap;
}

slice _sliceslicedefault(slice *v, int start, int end)
{
    if (start > end)
        error("slice start greater than end %d < %d", start, end);
    P(v->array);
    slice vec;
    _Zero(vec);
    vec.array = v->array;
    vec.start = v->start + start * v->array->elemSize;
    vec.len = end - start;
    return vec;
}

int _push(slice *v, void *p, int elemSize)
{
    if (elemSize != v->array->elemSize)
        error("not same type, got %d, need %d", elemSize, v->array->elemSize);
    _maybeEnlarge(v, elemSize);
    memcpy(v->start + v->len++ * elemSize, p, elemSize);
    return v->len;
}


int _popdefault(slice *v, void *p, int elemSize)
{
    if (v->len == 0)
        error("pop when length is 0");
    memcpy(p, v->start + --v->len * elemSize, elemSize);
    return v->len;
}
void _sliceget_(slice *v, int i, void *p, int elemSize)
{
    if (i < 0)
        error("index negative");
    if (i >= v->len)
        error("index out of range");
    memcpy(v->start + i * elemSize, p, elemSize);
}
void _slicesetdefault(slice *v, int i, void *p, int elemSize)
{
    if (i < 0)
        error("index negative");
    if (i >= v->len)
        error("index out of range");
    memcpy(p, v->start + i * elemSize, elemSize);
}

void _copy(slice *dst, int i, int j, slice *src, int k, int l, int size1, int size2)
{
    if (size1 != size2)
        error("size not equal %d!=%d when copy", size1, size2);
    if (strcmp(dst->array->type, src->array->type) != 0)
        error("type not equal %s %s", src->array->type, dst->array->type);
    if (dst == NULL)
        error("dst is NULL");
    if (src == NULL)
        error("src is NULL");
    if (i > j)
        error("index not possible");
    if (k > l)
        error("index not possible");
    if (i == j || k == l)
        return;
    int d1 = j - i;
    int d2 = l - k;
    int d = min(d1, d2);
    memmove(dst->start + i * size1, src->start + k * size2, size1 * d);
}
void sliceOnGc(slice *v)
{
    // free
}

_defsliceGet(int);
_defsliceGet(unsigned);
_defsliceGet(short);
_defsliceGet(long);
_defsliceGet(float);
_defsliceGet(double);
_defsliceGet(char);
_defsliceGet(byte);
_defsliceGet(bool);

_defslicePop(int);
_defslicePop(unsigned);
_defslicePop(short);
_defslicePop(long);
_defslicePop(float);
_defslicePop(double);
_defslicePop(char);
_defslicePop(byte);
_defslicePop(bool);

_defSliceSlicePrim(int);
_defSliceSlicePrim(unsigned);
_defSliceSlicePrim(short);
_defSliceSlicePrim(long);
_defSliceSlicePrim(float);
_defSliceSlicePrim(double);
_defSliceSlicePrim(char);
_defSliceSlicePrim(byte);
_defSliceSlicePrim(bool);

// ==== private ====
static void _maybeEnlarge(slice *v, int elemSize)
{
    if (v->array->cap == 0)
        _setArrayCap(v->array, _nextCapacity(0));
    else if (v->start + v->len == v->array->base + v->array->cap)
        _setArrayCap(v->array, _nextCapacity(v->array->cap));
}
static slice _makeslice_(array *a, char *typeStr, int len, int elemSize)
{
    slice v;
    _Zero(v);
    v.start = a->base;
    v.len = len;
    // setCustomFlag(v, isPrimitive(typeStr));
    return v;
}
static int _nextCapacity(int nowCapacity)
{
    return (int)((nowCapacity + enlargeIncrement) * enlargeFactor);
}
