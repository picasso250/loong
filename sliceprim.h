#ifndef _SLICEINT_H_
#define _SLICEINT_H_ 1

#include "util.h"
#include "gc.h"

#define _defArrayPrimitive(T) \
    typedef struct array##T   \
    {                         \
        GCFlag flag;          \
        char *type;           \
        int len;              \
        int elemSize;         \
        T *base;              \
    } array##T
#define _defSlicePrimitive(T) \
    typedef struct slice##T   \
    {                         \
        GCFlag flag;          \
        array##T *array;      \
        int len;              \
        T *start;             \
    } slice##T;

_defArrayPrimitive(int);
_defSlicePrimitive(int);
_defArrayPrimitive(unsigned);
_defSlicePrimitive(unsigned);
_defArrayPrimitive(short);
_defSlicePrimitive(short);
_defArrayPrimitive(long);
_defSlicePrimitive(long);
_defArrayPrimitive(float);
_defSlicePrimitive(float);
_defArrayPrimitive(double);
_defSlicePrimitive(double);
_defArrayPrimitive(char);
_defSlicePrimitive(char);
_defArrayPrimitive(byte);
_defSlicePrimitive(byte);
_defArrayPrimitive(bool);
_defSlicePrimitive(bool);

#define _defsliceGet(t)                              \
    t _getslice##t(slice##t *v, int i, int elemSize) \
    {                                                \
        t a;                                         \
        _sliceget_((slice *)v, i, &a, elemSize);     \
        return a;                                    \
    }                                                \
    typedef int _i__42

#define _defslicePop(T)                        \
    T _pop##T(slice##T *v)                     \
    {                                          \
        T aa;                                  \
        _slicepop((slice *)v, &aa, sizeof(T)); \
        return aa;                             \
    }                                          \
    typedef int _i__42

#define _defSliceSlicePrim(T)                                       \
    slice##T _sliceslice##T(slice##T v, int start, int end)         \
    {                                                               \
        slice vv = _sliceslicedefault((slice *)(&(v)), start, end); \
        return *((slice##T *)(&vv));                                \
    }                                                               \
    typedef int _i__42
#endif
