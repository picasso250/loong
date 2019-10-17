#include <stdlib.h>

#include "cval.h"
#include "util.h"

#define _make_map(...)                                    \
    _VEC_GET_MACRO(__VA_ARGS__, _make_map_3, _make_map_2) \
    (__VA_ARGS__)
#define _make_map_3(k, v, len) _make_map_(len)
#define _make_map_2(k, v) _make_map_3(k, v, 0)

cval *_make_map_(int len)
{
    cval *cval = alloc(cval);
    _set_type(cval, t_map);
    _zero(&(cval->v_map));
    _init_map(&(cval->v_map), len);
    return cval;
}
void _init_map(map *m, int len)
{
    m->len = len;
    if (len > 0)
    {
        m->base = calloc(len, sizeof(struct mapentry *));
        checkMem(m->base);
        memset(m->base, 0, len * sizeof(struct mapentry *));
    }
}