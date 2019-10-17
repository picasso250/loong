#ifndef BUILD_H
#define BUILD_H

#include "stringbuffer.h"

#define len(v) _Generic((v),                  \
                        stringbuffer          \
                        : stringbufferLen(v), \
                          str                 \
                        : strLen(v),          \
                          array               \
                        : arrayLen(v),        \
                          slice               \
                        : sliceLen(v),        \
                          map                 \
                        : mapLen(v), )

#define cap(v) _Generic((v),                  \
                        stringbuffer          \
                        : stringbufferCap(v), \
                          str                 \
                        : sliceCap(v),        \
                          slice               \
                        : sliceCap(v), )

#define copy(dst, src) _Generic((dst),                        \
                                stringbuffer                  \
                                : stringbufferCopy(dst, src), \
                                  str                         \
                                : sliceCopy(dst, src),        \
                                  slice                       \
                                : sliceCopy(dst, src))

#define make(t, ...) _Generic((t),         \
                              stringbuffer \
                              : stringbufferMake(__VA_ARGS__))

#define error(...)                                      \
    do                                                  \
    {                                                   \
        fprintf(stderr, "%s:%d: ", __FILE__, __LINE__); \
        fprintf(stderr, __VA_ARGS__);                   \
        fprintf(stderr, "\n");                          \
        abort();                                        \
    } while (0)

#endif