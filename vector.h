#ifndef VECTOR_H
#define VECTOR_H

#include <stdbool.h>

#include "gc.h"

typedef struct array
{
    GCFlag flag;
    char *type;
    int cap;
    int elemSize;
    void *base;
} array;
typedef struct vector
{
    GCFlag flag;
    char *type;
    array *array;
    int len;
    int elemSize;
    void *start;
} vector;

#define defArrayStructPri(t) \
    typedef struct array##t  \
    {                        \
        GCFlag flag;         \
        char *type;          \
        int cap;             \
        int elemSize;        \
        ##t *base;           \
    } array##t;
#define defArrayStruct(t)   \
    typedef struct array##t \
    {                       \
        GCFlag flag;        \
        char *type;         \
        int cap;            \
        int elemSize;       \
        ##t **base;         \
    } array##t;
#define defVectorStructPri(t) \
    typedef struct vector##t  \
    {                         \
        GCFlag flag;          \
        array##t *array;      \
        int len;              \
        ##t *start;           \
    } vector##t
#define defVectorStruct(t)   \
    typedef struct vector##t \
    {                        \
        GCFlag flag;         \
        array##t *array;     \
        int len;             \
        ##t **start;         \
    } vector##t
defArrayStructPri(int);
defVectorStructPri(int);

// make(vector,int,)
// makearray(int,0);

#define defSizeOf(t) \
    int _sizeof##t() { return sizeof(##t *); }
#define defSizeOfPri(t) \
    int _sizeof##t() { return sizeof(##t); }
#define defMakeArray(t)                                                          \
    array##t *_makearray##t(char *typeStr, array##t *arr, int len, int elemSize) \
    {                                                                            \
        return (array##t *)_makeArray_(typeStr, arr, len, elemSize);             \
    }
defMakeArray(int)
#define makearray(t, len) _makearray##t(#t, alloc(array##t), len, _sizeof##t())

#define makevector(t, len) _makeVectorCap(makearray(t, cap), #t, _sizeof##t(), len, cap)

#define defMakeVector(t)                                                                  \
    vector##t *_makevector##t(array##t *a, char *typeStr, int elemSize, int len, int cap) \
    {                                                                                     \
        return (vector##t *)_makeVectorCap(a, typeStr, elemSize, len, cap);               \
    }
    defMakeVector(int)

#define defVectorPush(t) \
    void _push##t(vector##t *v, t *e) { _push(v, &e, _sizeof##t()); }

#define defVectorPushPri(t) \
    void _push##t(vector##t *v, t e) { _push(v, &e, _sizeof##t()); }
        defVectorPushPri(int)
#define push(v, a) _push##typeof(a)

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
                          _primitiveDef(bool, __VA_ARGS__),  \
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

#define pop(a) _vectorpop##typeof(a)(_vector##typeof(a) a), sizeof(a->base[0]))

#define defVectorPop(t)                             \
    typedef int int_;##t _vectorpop##t(vector##t v, int elemSize) \
    {                                               \
        ##t aa;                                     \
        _vectorpop(v, &aa, elemSize);               \
        return aa;                                  \
    }
defVectorPop(int)
#endif
