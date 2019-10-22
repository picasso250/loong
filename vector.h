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

#define _sizeof(t) (isPrimitive(#t) ? sizeof(t) : sizeof(t *))

#define _makearray(t, len) _makearray(#t, alloc(array), len, _sizeof(t))

#define _makevector(t, len) _makeVectorCap(makearray(t, cap), #t, _sizeof(t), len, cap)

#define push(v, a) _push(v,a)

#define _getvector(t, v, i) _vector##t(#t, v, i, a, _sizeof##t())
#define defVectorGetPri(t)                                            \
    t _vectorGet##t(char *typeStr, vector##t *v, int i, int elemSize) \
    {                                                                 \
        if (strcmp(typeStr, #t) != 0)                                 \
            error("type error %d %d when vector get", typeStr, #t);   \
        t a;                                                          \
        _vectorGet(v, i, &a, elemSize);                               \
        return a;                                                     \
    }
        defVectorGetPri(int)
#define defVectorGet(t)                                                \
    t *_vectorGet##t(char *typeStr, vector##t *v, int i, int elemSize) \
    {                                                                  \
        if (strcmp(typeStr, #t) != 0)                                  \
            error("type error %d %d when vector get", typeStr, #t);    \
        t *a;                                                          \
        _vectorGet(v, i, &a, elemSize);                                \
        P(a);                                                          \
        return a;                                                      \
    }

#define vectorSet(v, i, a)                \
    do                                    \
    {                                     \
        void *p = &(a);                   \
        P(*p);                            \
        _vectorSet_(v, i, p, sizeof(*p)); \
    } while (0)
#define _vectorSet_(v, i, a, p) _Generic(a, primitiveDef(_vectorSet(v, i, p, sizeof(a))), \
                                         default                                          \
                                         : _vectorSet(v, i, a, P(*p)))
            int lenVector(vector *v);
int cap(vector *v);
vector *VectorCopy(vector *v);
vector *VectorSlice(vector *v, int start, int end);

#define pop(t, v) _vectorpop##t(#t, v, sizeof(v->base[0]), sizeof(t))

#define defVectorPop(t)                                                       \
    t _vectorpop##t(char *typeStr, vector##t *v, int elemSize, int elemSizeT) \
    {                                                                         \
        if (strcmp(typeStr, #t) != 0)                                         \
            error("type error %d %d when pop", typeStr, #t);                  \
        t aa;                                                                 \
        _vectorpop((vector *)v, &aa, elemSize, elemSizeT);                    \
        return aa;                                                            \
    }
defVectorPop(int)
#endif
