#include "map.h"
#include "hash.h"

static void _mapTypeCheck(map *m, char *type, const char *func)
{
    if (strcmp(type, m->type) != 0)
    {
        error("%s() map type mismatch %s %s", func, type, m->type);
    }
}
map *_mapMake(map *m, const char *typeStr, int elemSize, int factor)
{
    if (elemSize > sizeof(void *))
        error("not enough space for primitive type");
    _zero(m);
    m->type = typeStr;
    m->factor = factor;
    m->base = (mapentry **)calloc(factor, sizeof(mapentry *));
    checkMem(m->base);
    memset(m->base, 0, sizeof(mapentry *) * factor);
}

void *_mapGet(map *m, char *key, char *type, int elemSize)
{
    _mapTypeCheck(m, type, __func__);
    unsigned h = hashCStr(key, m->factor);
    unsigned index = h % m->factor;
    if (m->base[index] == NULL)
    {
        return nullMemory;
    }
    mapentry *ep = m->base[index];
    while (ep)
    {
        if (strcmp(ep->key, key) == 0)
        {
            return (ep->value);
        }
        ep = ep->next;
    }
    return nullMemory;
}
static mapentry *_newMapEntry(char *key, int elemSize, mapentry *next)
{
    mapentry *e = alloc(mapentry);
    e->key = key;
    e->value = malloc(elemSize);
    checkMem(e->value);
    e->next = next;
    return e;
}
static int _mapNextLen(int len)
{
    int arrLen = sizeof(primeFactorList) / sizeof(primeFactorList[0]);
    int max = primeFactorList[arrLen - 1];
    if (len > max)
        return max;
    for (int i = 0; i < arrLen; i++)
    {
        if (len < primeFactorList[i])
            return primeFactorList[i];
    }
    return max;
}
static void _mapEnlarge(map *m, int newFactor)
{

    mapentry **tmp = calloc(m->len, sizeof(mapentry *));
    checkMem(tmp);
    int i = 0;
    formap(m, k, int, _)
    // for (int _i_ = 0; _i_ < m->factor; _i_++)
    // {
    //     for (mapentry *_pme_ = m->base[_i_]; _pme_; _pme_ = _pme_->next)
    //     {
    //         char *k = _pme_->key;
    //         int _ = *(int *)_pme_->value;
            tmp[i++] = _pme_;
    //     }
    // }
    endformap;

    m->factor = newFactor;
    m->base = realloc(m->base, sizeof(mapentry *) * m->factor);
    checkMem(m->base);
    memset(m->base, 0, sizeof(mapentry *) * newFactor);

    for (i = 0; i < m->len; i++)
    {
        mapentry *e = tmp[i];
        unsigned h = hashCStr(e->key, m->factor);
        unsigned index = h % m->factor;
        if (m->base[index] == NULL)
        {
            m->base[index] = e;
        }
        else
        {
            e->next = m->base[index];
            m->base[index] = e;
        }
    }
    free(tmp);
}
static void _mapEnlargeMaybe(map *m)
{
    if (m->len > m->factor * 2)
    {
        int newf = _mapNextLen(m->factor * 2);
        if (newf > m->factor)
        {
            _mapEnlarge(m, newf);
        }
    }
}
void *_mapSet(map *m, char *key, char *type, int elemSize)
{
    _mapTypeCheck(m, type, __func__);

    _mapEnlargeMaybe(m);

    unsigned h = hashCStr(key, m->factor);
    unsigned index = h % m->factor;

    if (m->first == NULL)
    {
        m->first = m->base[index] = _newMapEntry(key, elemSize, NULL);
        m->len++;
        return m->base[index]->value;
    }
    if (m->base[index] == NULL)
    {
        m->base[index] = _newMapEntry(key, elemSize, NULL);
        m->len++;
    }
    else
    {
        mapentry *ep = m->base[index];
        while (ep)
        {
            if (strcmp(ep->key, key) == 0)
            {
                return ep->value;
            }
            ep = ep->next;
        }
        // not found, insert to head
        m->base[index] = _newMapEntry(key, elemSize, m->base[index]);
        m->len++;
    }
    return m->base[index]->value;
}
static void _mapentryFree(mapentry *e)
{
    if (e == NULL)
        error("free null mapentry");
    if (e->value == NULL)
        error("free null mapentry value");
    free(e->value);
    free(e);
}
int _mapDel(map *m, char *key)
{
    unsigned h = hashCStr(key, m->factor);
    unsigned index = h % m->factor;
    if (m->base[index] == NULL)
    {
        return 0;
    }
    mapentry *ep = m->base[index];
    mapentry *prev = NULL;
    while (ep)
    {
        if (strcmp(ep->key, key) == 0)
        {
            if (prev == NULL)
            {
                m->base[index] = ep->next;
            }
            else
            {
                prev->next = ep->next;
            }
            m->len--;
            _mapentryFree(ep);
            return 1;
        }
        prev = ep;
        ep = ep->next;
    }
    return 0;
}
void _mapGraph(map *m, char *fmt)
{
    printf("Map %X\n",m);
    for (int _i_ = 0; _i_ < m->factor; _i_++)
    {
        printf("\t%s", m->base[_i_] ? "-:> " : "NULL");
        for (mapentry *_pme_ = m->base[_i_]; _pme_; _pme_ = _pme_->next)
        {
            char *k = _pme_->key;
            int v = *(int *)_pme_->value;
            printf("(%s->%d) -> ", k, v);
        }
        printf("\n");
    }
}