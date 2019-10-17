#include <stdlib.h>
#include <stdbool.h>

#include "cval.h"
#include "util.h"

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
    m->len = m->factor = len;
    if (len <= 0)
    {
        len = m->factor = 7;
    }
    m->base = calloc(len, sizeof(struct mapentry *));
    checkMem(m->base);
    memset(m->base, 0, len * sizeof(struct mapentry *));
}

void mapSet(map *m, cval *k, cval *v)
{
    if (m->len > 2 * m->factor)
    {
        int newFactor = m->factor * 2 + 1;

        //todo
    }
    //todo P(k)P(v)
    unsigned h = hash(k);
    int index = h % m->factor;

    mapentry *head = m->base[index];
    mapentry *e = _makeMapEntry(k, v);
    if (head == NULL)
    {
        m->base[index] = e;
    }
    else
    {
        //search
        mapentry *tail = _linklist_tail(head);
        tail->next = e;
    }
}
mapentry *_makeMapEntry(cval *key, cval *value)
{
    mapentry *e = alloc(mapentry);
    e->key = key;
    e->value = value;
    return e;
}
mapentry *_linklist_tail(mapentry *head)
{
    notNull(head);
    mapentry *tail = head;
    for (;head; head = head->next)
        tail = head;
    return tail;
}
unsigned hash(cval *v)
{
    switch (type(v))
    {
    case t_int:
        return v->v_int;
    case t_float:
        return _hash_float(v->v_float);
    case t_byte:
        return _hash_byte(v->v_byte);
    case t_bool:
        return _hash_bool(v->v_bool);
    case t_str:
        return _hash_str(v->v_str);
    case t_array:
        error("array can not be hashed");
    case t_slice:
        error("slice can not be hashed");
    case t_map:
        error("map can not be hashed");
    default:
        shouldNotHere();
    }
}

unsigned _hash_float(float v)
{
    unsigned a;
    memcpy(&a, &v, sizeof(a));
    return a;
}
unsigned _hash_byte(byte v)
{
    return v;
}
unsigned _hash_bool(bool v)
{
    return v;
}
unsigned _hash_str(str v)
{
    const int p = 31;
    const int m = 1e9 + 9;
    long long hash_value = 0;
    long long p_pow = 1;
    _Generic((v),
             array
             : arrayLen(v),
               slice
             : sliceLen(v),
               map
             : mapLen(v));
    for (int i = 0; i < len(v); i++)
    {
        char c = v.base->v_array.base[i];
        hash_value = (hash_value + (c - 'a' + 1) * p_pow) % m;
        p_pow = (p_pow * p) % m;
    }
    return hash_value;
}
