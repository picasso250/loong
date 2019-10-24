#include <stdbool.h>

#include "gc.h"

void _setSweepFlag(GCFlag *flag, unsigned v)
{
    *flag = v ? *flag | sweepMask : *flag & ~sweepMask;
}
void _setCustomFlag(GCFlag *flag, unsigned v)
{
    *flag = v ? *flag | customMask : *flag & ~customMask;
}

static bool _isRefCountZero(GCFlag f)
{
    return (f & refCountMask) == 0;
}
 GCFlag _incr(GCFlag *f)
{
    if ((*f | ~refCountMask) == ~0U)
        error("ref count overflow");
    return ++*f;
}
 GCFlag _decr(GCFlag *f)
{
    if (*f & refCountMask == 0U)
        error("ref count is zero when releasing");
    return --*f;
}
void *_V_step0(void *a)
{
    if (a == NULL)
        error("gc NULL");
    GCFlag f = _decr((GCFlag *)a);
    return _isRefCountZero(f) ? a : NULL;
}
// first decr
// then onGC final
// last thing, free it self
int _V_step2(void *p)
{
    if (p)
        free(p);
    return 0;
}