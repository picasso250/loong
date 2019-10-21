#include <stdbool.h>

#include "util.h"

typedef unsigned GCFlag;

typedef struct _box
{
    GCFlag flag;
    void *data;
    int size;
} _box;

#define refCountMask (~0U >> 1)

#define P(a) _Generic(a,                                              \
                      defP(int), defP(float), defP(byte), defP(bool), \
                      default                                         \
                      : _P(&(a)->flag))
#define defP(t) \
    t:          \
    __PPrimary(alloc(t), sizeof(t))

int _P(GCFlag *flag)
{
    if (*flag | ~refCountMask == ~0U)
        error("ref count overflow");
    *flag++;
}
_box* _PPrimary(void *p, int size)
{
    _box *b = alloc(_box);
    P(b);
    b->data = p;
    return b;
}

#define V(a) _Generic(a,                                              \
                      defV(int), defV(float), defV(byte), defV(bool), \
                      default                                         \
                      : _V(&(a)->flag))
// #define defP(t) \
//     t:          \
//     __PPrimary(alloc(t), sizeof(t))

int _V(GCFlag *flag)
{
    if (*flag & refCountMask == 0U)
        error("ref count is zero when releasing");
    return --(*flag);
}