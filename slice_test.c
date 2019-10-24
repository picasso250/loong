#include "test.h"

#include "slice.h"
#include "buildin.h"

void testSlice()
{
    slice a ;
    initslice(&a,int, 0,0);
    push(&a, int, 42);
    int d = len(a);
    testEqual(d, 1);
    push(&a, int, 43);
    int b;
    pop(&a, b);
    testEqual(b, 43);
    int c;
    get(&a, 0, c);
    testEqual(c, 42);
    set(&a, int, 0, 55);
    testEqual(*(int *)a.array->base, 55);
    int f;
    get(&a, 0, f);
    testEqual(f, 55);
    int cc=cap(&a);
    testEqual(cc, 7);
    push(&a, int, 11);
    push(&a, int, 22);
    push(&a, int, 33);
    push(&a, int, 44);
    push(&a, int, 55);
    push(&a, int, 66);
    push(&a, int, 77);
    push(&a, int, 88);
    testEqual(cap(&a), 19);
    slice e = sl(&a, 1, 3);
    int g;
    get(&e, 0, g);
    testEqual(g, 11);
}