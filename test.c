#include <stdio.h>

#include "test.h"

#include "buildin.h"

#include ".test_decl.h"

int main(int argc, char *argv[])
{
    #include ".test_exec.c"
    printf("OK.\n");
}