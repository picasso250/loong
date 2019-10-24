#ifndef _GC_H
#define _GC_H 1
#include <stdbool.h>

#include "util.h"
#include "type.h"

// for example 32
// 32            31             30 ...... 2 1 0
//  ^--sweepFlag  ^--customFlag  ^--refCount--^
// typedef unsigned GCFlag;

#define sweepMask (~(~0U >> 1))
#define customMask (~(~0U >> 1) >> 1)
#define refCountMask (~0U >> 2)

#define sweepFlag(a) ((a)->flag & sweepMask)
#define customFlag(a) ((a)->flag & customMask)
#define refCount(a) ((a)->flag & refCountMask)

#define setSweepFlag(a, v) _setSweepFlag(&(a)->flag, v)
#define setCustomFlag(a, v) _setCustomFlag(&(a)->flag, v)

 GCFlag _incr(GCFlag *f);
 GCFlag _decr(GCFlag *f);
static bool _isRefCountZero(GCFlag f);

// occupy
#define P(p) _incr(&(p)->flag)

// first decr
// then onGC final
// last thing, free it self
void *_V_step0(void *a);
int _V_step2(void *p);

// if NULL is passed, nothing should be done
// if NULL is returned, nothing will be free
array*arrayfinal(array*a);
slice*slicefinal(slice*s);
map*mapfinal(map*m);

// release
#define V(a) _Generic(a,                                   \
                      array *                              \
                      : _V_step2(arrayfinal(_V_step0(a))), \
                        slice *                            \
                      : _V_step2(slicefinal(_V_step0(a))), \
                        map *                              \
                      : _V_step2(mapfinal(_V_step0(a))))

#endif
