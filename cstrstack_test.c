#include <stdbool.h>

#include "test.h"
#include "cstrstack.h"

void testIt()
{
    cstrstack s = cssnew();
    testEqual(cssempty(s), true);
    s=csspush(s, "hello");
    testEqual(strcmp(csspop(s), "hello"), 0);
    testEqual(cssempty(s), true);
}