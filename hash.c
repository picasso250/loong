
#include "hash.h"

unsigned hashCStr(char *s, int p)
{
    // const int p = 31;
    const int m = 1e9 + 9;
    long long hash_value = 0;
    long long p_pow = 1;
    for (; *s; s++)
    {
        char c = *s;
        hash_value = (hash_value + (c - 'a' + 1) * p_pow) % m;
        p_pow = (p_pow * p) % m;
    }
    return hash_value;
}

#define BIG_ENOUGH_PRIME_FOR_SYMBOL 999931
unsigned hashSymbol(char *s)
{
    return hashCStr(s, BIG_ENOUGH_PRIME_FOR_SYMBOL);
}

// #include <stdio.h>
// int main()
// {
//     printf("int %d\n", hashSymbol("int"));
//     printf("float %d\n", hashSymbol("float"));
//     printf("byte %d\n", hashSymbol("byte"));
//     printf("bool %d\n", hashSymbol("bool"));
// }