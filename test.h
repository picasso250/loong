#ifndef _TEST_H_
#define _TEST_H_ 1

#include "util.h"
#include "gc.h"

#define testEqual(really, should)                                                     \
    do                                                                                \
    {                                                                                 \
        if (really != should)                                                         \
        {                                                                             \
            fprintf(stderr, "%s:%d: %s() not equal: ", __FILE__, __LINE__, __func__); \
            fprintf(stderr, format_identifier(really), really);                       \
            fprintf(stderr, " ");                                                     \
            fprintf(stderr, format_identifier(should), should);                       \
            fprintf(stderr, "\n");                                                    \
            abort();                                                                  \
        }                                                                             \
    } while (0)

#define testTrue(cond)                                                               \
    do                                                                               \
    {                                                                                \
        if (!cond)                                                                   \
        {                                                                            \
            fprintf(stderr, "%s:%d: %s() not true\n", __FILE__, __LINE__, __func__); \
            abort();                                                                 \
        }                                                                            \
    } while (0)

#endif