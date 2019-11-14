#ifndef _TEST_H_
#define _TEST_H_ 1

#include "util.h"

#define testEqual(really, should)                                                                              \
    do                                                                                                         \
    {                                                                                                          \
        if ((really) != (should))                                                                              \
        {                                                                                                      \
            fprintf(stderr, "./%s:%d %s() %s %s not equal: ", __FILE__, __LINE__, __func__, #really, #should); \
            fprintf(stderr, "\n");                                                                             \
            abort();                                                                                           \
        }                                                                                                      \
    } while (0)

#define testTrue(cond)                                                                           \
    do                                                                                           \
    {                                                                                            \
        if (!(cond))                                                                             \
        {                                                                                        \
            fprintf(stderr, "./%s:%d: %s() %s not true\n", __FILE__, __LINE__, __func__, #cond); \
            abort();                                                                             \
        }                                                                                        \
    } while (0)

#endif