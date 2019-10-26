#include "map.h"
#include "hash.h"

static int _mapNextLen(int len);

static void _mapTypeCheck(map *m, char *type, const char *func)
{
    if (strcmp(type, m->type) != 0)
    {
        error("%s() map type mismatch %s %s", func, type, m->type);
    }
}
map *_mapMake(map *m, const char *typeStr, int elemSize, int len)
{
    if (elemSize > sizeof(void *))
        error("not enough space for primitive type");
    _zero(m);
    m->type = typeStr;
    m->elemSize = elemSize;
    m->factor = _mapNextLen(len);
    m->base = (mapentry **)calloc(m->factor, sizeof(mapentry *));
    checkMem(m->base);
    memset(m->base, 0, sizeof(mapentry *) * m->factor);
}

void *_mapGet(map *m, char *key, char *type)
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
        tmp[i++] = _pme_;
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
            e->next = NULL;
        }
        else
        {
            e->next = m->base[index];
            m->base[index] = e;
        }
    }
    free(tmp);
    // printf("rehash ");
    // _mapGraph(m, "%d");
}
static void _mapEnlargeMaybe(map *m)
{
    if (m->len > m->factor)
    {
        int newf = _mapNextLen(m->factor);
        if (newf > m->factor)
        {
            _mapEnlarge(m, newf);
        }
    }
}
void *_mapSet(map *m, char *key, char *type)
{
    _mapTypeCheck(m, type, __func__);

    _mapEnlargeMaybe(m);

    unsigned h = hashCStr(key, m->factor);
    unsigned index = h % m->factor;

    if (m->first == NULL)
    {
        m->first = m->base[index] = _newMapEntry(key, m->elemSize, NULL);
        m->len++;
        return m->base[index]->value;
    }
    if (m->base[index] == NULL)
    {
        m->base[index] = _newMapEntry(key, m->elemSize, NULL);
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
        m->base[index] = _newMapEntry(key, m->elemSize, m->base[index]);
        m->len++;
    }
    return m->base[index]->value;
}
static void _mapentryFree(mapentry *e, int elemSize)
{
    if (e == NULL)
        error("free null mapentry");
    if (e->value == NULL)
        error("free null mapentry value");
    memset(e->value, 0, elemSize);
    free(e->value);
    e->value = NULL;
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
            _mapentryFree(ep, m->elemSize);
            return 1;
        }
        prev = ep;
        ep = ep->next;
    }
    return 0;
}
void _mapGraph(map *m, char *fmt)
{
    printf("Map len=%d factor=%d 0x%X\n", m->len, m->factor, m);
    for (int _i_ = 0; _i_ < m->factor; _i_++)
    {
        printf("\t%s", m->base[_i_] ? "-:> " : "NULL");
        for (mapentry *_pme_ = m->base[_i_]; _pme_; _pme_ = _pme_->next)
        {
            char *k = _pme_->key;
            int v = *(int *)_pme_->value;
            char buf[100] = "";
            sprintf(buf, "(%s: %s) -> ", "\"%s\"", fmt);
            printf(buf, k, v);
        }
        printf("\n");
    }
}
map *mapfinal(map *m)
{
    if (m)
    {
        for (int i = 0; i < m->factor; i++)
        {
            mapentry *p = m->base[i];
            while (p)
            {
                mapentry *pp = p->next;
                p->next = NULL;
                _mapentryFree(p, m->elemSize);
                p = pp;
            }
        }
        free(m->base);
        m->base = NULL;
    }
}