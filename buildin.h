#ifndef BUILD_H
#define BUILD_H

// #include "str.h"
#include "slice.h"
#include "map.h"

#define len(v) ((v)->len)

#define make(t, ...) make##t(__VA_ARGS__)

#define get(c, ...) _Generic(c,                                    \
                             slice *                               \
                             : _getslice((slice *)c, __VA_ARGS__), \
                               map *                               \
                             : _getmap((map *)c, __VA_ARGS__))
#define set(c, ...) _Generic(c,                                    \
                             slice *                               \
                             : _setslice((slice *)c, __VA_ARGS__), \
                               map *                               \
                             : _setmap((map *)c, __VA_ARGS__))

#endif