#include "map.h"
#include "buildin.h"
#include "test.h"

void testMap() {
    map m;
    initmap(&m,int);
    set(&m,"a",int,2);testEqual(m.len,1);
    int b=get(&m,"b",int);
    testEqual(b,0);
    testEqual(get(&m,"a",int),2);
    set(&m,"b",int,42);
    testEqual(get(&m,"b",int),42);
    del(&m,"b");
    testEqual(get(&m,"b",int),0);testEqual(m.len,1);
    set(&m,"c",int,33);
    _mapGraph(&m,"%d");
    set(&m,"d",int,44);
    _mapGraph(&m,"%d");
    set(&m,"e",int,55);
    _mapGraph(&m,"%d");
    set(&m,"f",int,66);
    _mapGraph(&m,"%d");
    // testEqual(m.factor,5);
    set(&m,"g",int,77);
    _mapGraph(&m,"%d");
    set(&m,"h",int,88);
    _mapGraph(&m,"%d");
}