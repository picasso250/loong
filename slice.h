#ifndef _SLICE_H_
#define _SLICE_H_ 1

#include "util.h"
#include "gc.h"
#include "type.h"

#include "sliceprim.h"

typedef struct array
{
    GCFlag flag;
    char *type;
    int cap;
    int elemSize;
    char *base; // T[] not T*[]
} array;
// slice can be a value type, not a ref type
typedef struct slice
{
    GCFlag flag;
    array *array;
    int len;
    char *start;
} slice;

/*
 * there are two ways to check type, #t and _Generic
 * later be try first, then #t
 * types are checked dynamically
 */
#define _sizeof(t) (isPrimitive(#t) ? sizeof(t) : sizeof(t *))

#define _makearray(T, len) _makearray_(#T, alloc(array), len, sizeof(T))

#define _makeslice(t, len) _makesliceCap(makearray(t, cap), #t, _sizeof(t), len, cap)

#define cap(v) ((v)->array->len)

#define push(v, a) _push(v, a, sizeof(a),P(a))

#define pop(v) _Generic((v)->start[0],     \
                        int                \
                        : _popint(v),      \
                          unsigned         \
                        : _popunsigned(v), \
                          short            \
                        : _popshort(v),    \
                          long             \
                        : _poplong(v),     \
                          float            \
                        : _popfloat(v),    \
                          double           \
                        : _popdouble(v),   \
                          char             \
                        : _popchar(v),     \
                          byte             \
                        : _popbyte(v),     \
                          bool             \
                        : _popbool(v),     \
                          default          \
                        : (typeof((v)->start[0]) _popdefault(v)))

#define _getslice(v, i) _Generic((v)->start[0],                               \
                                 int                                          \
                                 : _getsliceint(v, i, sizeof(int)),           \
                                   unsigned                                   \
                                 : _getsliceunsigned(v, i, sizeof(unsigned)), \
                                   short                                      \
                                 : _getsliceshort(v, i, sizeof(short)),       \
                                   long                                       \
                                 : _getslicelong(v, i, sizeof(long)),         \
                                   float                                      \
                                 : _getslicefloat(v, i, sizeof(float)),       \
                                   double                                     \
                                 : _getslicedouble(v, i, sizeof(double)),     \
                                   char                                       \
                                 : _getslicechar(v, i, sizeof(char)),         \
                                   byte                                       \
                                 : _getslicebyte(v, i, sizeof(byte)),         \
                                   bool                                       \
                                 : _getslicebool(v, i, sizeof(bool)),         \
                                   default                                    \
                                 : (typeof((v)->start[0]) *)_getslicedefault(v, i, sizeof(char *)))

#define _sliceset(v, i, a) _Generic(a,                                   \
                                    int                                  \
                                    : _slicesetprim(v, i, a, sizeof(a)), \
                                      unsigned                           \
                                    : _slicesetprim(v, i, a, sizeof(a)), \
                                      short                              \
                                    : _slicesetprim(v, i, a, sizeof(a)), \
                                      long                               \
                                    : _slicesetprim(v, i, a, sizeof(a)), \
                                      float                              \
                                    : _slicesetprim(v, i, a, sizeof(a)), \
                                      double                             \
                                    : _slicesetprim(v, i, a, sizeof(a)), \
                                      char                               \
                                    : _slicesetprim(v, i, a, sizeof(a)), \
                                      byte                               \
                                    : _slicesetprim(v, i, a, sizeof(a)), \
                                      bool                               \
                                    : _slicesetprim(v, i, a, sizeof(a)), \
                                      default                            \
                                    : _slicesetdefault(v, i, a, sizeof(a)))

#define sl(v, b, e) _Generic((v)->start[0],                  \
                                      int                             \
                                      : _slicesliceint(v, b, e),      \
                                        unsigned                      \
                                      : _slicesliceunsigned(v, b, e), \
                                        short                         \
                                      : _slicesliceshort(v, b, e),    \
                                        long                          \
                                      : _sliceslicelong(v, b, e),     \
                                        float                         \
                                      : _sliceslicefloat(v, b, e),    \
                                        double                        \
                                      : _sliceslicedouble(v, b, e),   \
                                        char                          \
                                      : _sliceslicechar(v, b, e),     \
                                        byte                          \
                                      : _sliceslicebyte(v, b, e),     \
                                        bool                          \
                                      : _sliceslicebool(v, b, e),     \
                                        default                       \
                                      : _sliceslicedefault((slice*)v, b, e))

#define copy(dst, i, j, src, k, l) _copy(dst, i, j, src, k, l, dst->array->elemSize, src->array->elemSize)

#endif