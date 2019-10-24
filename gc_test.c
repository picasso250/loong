#include "test.h"
#include "type.h"
#include "slice.h"
#include "map.h"

void testGCArray()
{
    array a;
    initarray(&a, int, 0);
    testEqual(a.flag, 0);
    P(&a);
    testEqual(a.flag, 1);

    slice *s = newslice(int, 0, 5);
    P(s);
    testEqual(s->flag, 1);
    testEqual(s->array->flag, 1);
    V(s);
    // testEqual(s->array->flag, 0);
    // testEqual(s->flag, 0);
    V(&a);

    s = newslice(int, 0, 5);
    P(s);
    push(s,int,11);
    V(s);

}