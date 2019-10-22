#ifndef _SLICEINT_H_
#define _SLICEINT_H_ 1

#include "util.h"
#include "gc.h"

#define _defArrayPrimitive(T) \
typedef struct array##T \
{ \
    GCFlag flag; \
    char *type; \
    int cap; \
    int elemSize; \
    T *base; \
} array##T
#define _defSlicePrimitive(T) \
typedef struct slice##T \
{ \
    GCFlag flag; \
    char *type; \
    array##T *array; \
    int len; \
    int elemSize; \
    T *start; \
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

#define _getslice(v, i)                               \
    _Generic( \
    *((v)->start)), \
    int:_getsliceint(v,i,sizeof(int)), \
    unsigned:_getsliceunsigned(v,i,sizeof(unsigned)), \
    short:_getsliceshort(v,i,sizeof(short)), \
    long:_getslicelong(v,i,sizeof(long)), \
    float:_getslicefloat(v,i,sizeof(float)), \
    double:_getslicedouble(v,i,sizeof(double)), \
    char:_getslicechar(v,i,sizeof(char)), \
    byte:_getslicebyte(v,i,sizeof(byte)), \
    bool:_getslicebool(v,i,sizeof(bool)), \
    default:_getslice_(v,i,sizeof(char*)))

#define _defsliceGet(t)                            \
    t _getslice##t(slice##t *v, int i, int elemSize) \
    {                                                  \
        t a;                                           \
        _sliceGet(v, i, &a, elemSize);                \
        return a;                                      \
    }

#define _defslicePop(T)                                                       \
    T _slicepop##T( slice##T *v, int elemSize) \
    {                                                                         \
        T aa;                                                                 \
        _slicepop((slice *)v, &aa, elemSize);                    \
        return aa;                                                            \
    }
    

#define _copy(dst, i, j, src, k, l,size1,size2) _Generic(*v, \
sliceint:_copyint(dst, i, j, src, k, l,size1,size2), \
sliceunsigned:_copyunsigned(dst, i, j, src, k, l,size1,size2), \
sliceshort:_copyshort(dst, i, j, src, k, l,size1,size2), \
slicelong:_copylong(dst, i, j, src, k, l,size1,size2), \
slicefloat:_copyfloat(dst, i, j, src, k, l,size1,size2), \
slicedouble:_copydouble(dst, i, j, src, k, l,size1,size2), \
slicechar:_copychar(dst, i, j, src, k, l,size1,size2), \
slicebyte:_copybyte(dst, i, j, src, k, l,size1,size2), \
slicebool:_copybool(dst, i, j, src, k, l,size1,size2), \
default:_copydefault(dst, i, j, src, k, l,size1,size2), \
)
slice *sliceCopy(slice *v);
slice *sliceSlice(slice *v, int start, int end);



#endif

int
unsigned
short
long
float
double
char
byte
bool
default