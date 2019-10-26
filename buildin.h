#ifndef BUILD_H
#define BUILD_H

// #include "str.h"
#include "slice.h"
#include "str.h"
#include "map.h"

#define len(v) ((v)->len)

#define make(t, ...) make##t(__VA_ARGS__)

// c, i, T
#define get(c, i, T) _Generic(c,                                                  \
                              array *                                             \
                              : (T) * (T *)_getarrayp((array *)(c), (int)(i), T), \
                                slice *                                           \
                              : (T) * (T *)_getslicep((slice *)(c), (int)(i), T), \
                                map *                                             \
                              : (T) * (T *)_getmapp((map *)(c), (char *)(i), T))
#define getp(c, i, T) _Generic(c,                                            \
                               array *                                       \
                               : (T *)_getarrayp((array *)(c), (int)(i), T), \
                                 slice *                                     \
                               : (T *)_getslicep((slice *)(c), (int)(i), T), \
                                 map *                                       \
                               : (T *)_getmapp((map *)(c), (char *)(i), T))
// c, i, T, e
#define set(c, i, T, e) _Generic(c,                                         \
                                 array *                                    \
                                 : _setarray((array *)(c), (int)(i), T, e), \
                                   slice *                                  \
                                 : _setslice((slice *)(c), (int)(i), T, e), \
                                   map *                                    \
                                 : _setmap((map *)(c), (char *)(i), T, e))

#endif