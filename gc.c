#include "gc.h"

void _setSweepFlag(GCFlag *flag, unsigned v)
{
    *flag = v ? *flag | sweepMask : *flag & ~sweepMask;
}
void _setCustomFlag(GCFlag *flag, unsigned v)
{
    *flag = v ? *flag | customMask : *flag & ~customMask;
}
void *_P(void *p)
{
    GCFlag *flag = (GCFlag *)p;
    if ((*flag | ~refCountMask) == ~0U)
        error("ref count overflow");
    *flag += 1;
    return p; // for define
}