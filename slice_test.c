#include "slice.h"
#include "buildin.h"

void testSlice(){
    sliceint a=make(slice,int,0);
    push(a,42);
    int d=len(a);
    testEqual(d,1);
    push(a,43);
    int b=pop(a);
    testEqual(b,43);
    int c=get(a,0);
    testEqual(c,42);
    set(a,0,55);
    testEqual(get(a,0),55);
    testEqual(cap(a),7);
    push(a,11);
    push(a,22);
    push(a,33);
    push(a,44);
    push(a,55);
    push(a,66);
    sliceint *e=sl(a,1,3);
    testEqual(get(e,0),11);

}