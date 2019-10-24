#include <stdio.h>

#include "util.h"
#include "test.h"

#include "slice.h"
#include "slice_test.c"
#include "map_test.c"

int main(int argc, char *argv[])
{
    testSlice();
    testMap();
    printf("OK.\n");
}