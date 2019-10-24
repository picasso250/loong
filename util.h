#ifndef _UTIL_H
#define _UTIL_H 1

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define format_identifier(v) _Generic(v,        \
                                      char *    \
                                      : "%s",   \
                                        int     \
                                      : "%d",   \
                                        long    \
                                      : "%ld",  \
                                        default \
                                      : "%d")

#define assertEqual(name, v)                                                    \
    do                                                                          \
    {                                                                           \
        if (name != v)                                                          \
        {                                                                       \
            fprintf(stderr, "%s:%d: %s should be ", __FILE__, __LINE__, #name); \
            fprintf(stderr, format_identifier((v)), (v));                       \
            fprintf(stderr, ", but got ");                                      \
            fprintf(stderr, format_identifier((v)), name);                      \
            fprintf(stderr, "\n");                                              \
            abort();                                                            \
        }                                                                       \
    } while (0)

#define notNull(p)                                                                     \
    do                                                                                 \
    {                                                                                  \
        if (!p)                                                                        \
        {                                                                              \
            fprintf(stderr, "%s:%d: %s should not be null\n", __FILE__, __LINE__, #p); \
            abort();                                                                   \
        }                                                                              \
    } while (0)

#define assertType(v, t)                                                                                    \
    do                                                                                                      \
    {                                                                                                       \
        if (type(v) != t)                                                                                   \
        {                                                                                                   \
            fprintf(stderr, "%s:%d: %s should be %s, but got %s\n", __FILE__, __LINE__, #v, #t, (type(v))); \
            abort();                                                                                        \
        }                                                                                                   \
    } while (0)

#define checkMem(p)                                                                              \
    do                                                                                           \
    {                                                                                            \
        if (!p)                                                                                  \
        {                                                                                        \
            fprintf(stderr, "%s:%d: memory allocation failed for %s\n", __FILE__, __LINE__, #p); \
            abort();                                                                             \
        }                                                                                        \
    } while (0)

#define alloc(T) ((T *)_alloc_and_zero(sizeof(T), __FILE__, __LINE__, #T))

void *_alloc_and_zero(int size, char *file, int line, char *t);
#define new(T) ((T *)_alloc_and_zero(sizeof(T), __FILE__, __LINE__, #T))

// z for pointer, Z for struct
#define _zero(p) memset(p, 0, sizeof(*(p)))
#define _Zero(a) memset(&(a),0,sizeof(a))

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

#endif
