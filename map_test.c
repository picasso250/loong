#include "map.h"
#include "buildin.h"
#include "test.h"

void testMap() {
    map m;
    initmap(&m,int);
    set(&m,"a",int,2);
    int b=get(&m,"b",int);
    testEqual(b,0);
    testEqual(get(&m,"a",int),2);
    set(&m,"b",int,42);
    testEqual(get(&m,"b",int),42);
    del(&m,"b");
    testEqual(get(&m,"b",int),0);
}