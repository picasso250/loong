#ifndef _GC_H
#define _GC_H 1
#include <stdbool.h>

#include "util.h"
#include "type.h"

// for example 32
// 32            31             30 ...... 2 1 0
//  ^--sweepFlag  ^--customFlag  ^--refCount--^
typedef unsigned GCFlag;

#define sweepMask (~(~0U >> 1))
#define customMask (~(~0U >> 1) >> 1)
#define refCountMask (~0U >> 2)

#define sweepFlag(a) ((a)->flag & sweepMask)
#define customFlag(a) ((a)->flag & customMask)
#define refCount(a) ((a)->flag & refCountMask)

#define setSweepFlag(a, v) _setSweepFlag(&(a)->flag, v)
#define setCustomFlag(a, v) _setCustomFlag(&(a)->flag, v)

void *_P(void *p);
#define P(p) _Generic(p,         \
                      int        \
                      : p,       \
                        unsigned \
                      : p,       \
                        short    \
                      : p,       \
                        long     \
                      : p,       \
                        float    \
                      : p,       \
                        double   \
                      : p,       \
                        char     \
                      : p,       \
                        byte     \
                      : p,       \
                        bool     \
                      : p,       \
                        default  \
                      : _P(p))

static GCFlag incr(GCFlag f)
{
    if ((f | ~refCountMask) == ~0U)
        error("ref count overflow");
    return f + 1;
}
static GCFlag decr(GCFlag f)
{
    if (f & refCountMask == 0U)
        error("ref count is zero when releasing");
    return f - 1;
}
#define V(a) _Generic(a,                                              \
                      defV(int), defV(float), defV(byte), defV(bool), \
                      default                                         \
                      : _V0(&a->flag, &a, a->flag - 1 ? _V2(&a->flag) : a##final(a, _V2(&a->flag))))
#define defV(t) \
    t:          \
    0

// then flag--
int _V2(GCFlag *flag)
{
    if (*flag & refCountMask == 0U)
        error("ref count is zero when releasing");
    *flag--;
    return 0;
}
// then onGC
int _V0(GCFlag *flag, void *p, int cont) // last thing, free it self
{
    if (*flag == 0)
        free(p);
    return 0;
}
#endif
