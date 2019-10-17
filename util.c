
#include <stdlib.h>
#include <stdio.h>

#define _format_identifier(v) _Generic(v,        \
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
            fprintf(stderr, _format_identifier((v)), (v));                      \
            fprintf(stderr, ", but got ");                                      \
            fprintf(stderr, _format_identifier((v)), name);                     \
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

#define assertType(v, t)                                                                                           \
    do                                                                                                             \
    {                                                                                                              \
        if (type(v) != t)                                                                                          \
        {                                                                                                          \
            fprintf(stderr, "%s:%d: %s should be %s, but got %s\n", __FILE__, __LINE__, #v, #t, typeStr(type(v))); \
            abort();                                                                                               \
        }                                                                                                          \
    } while (0)