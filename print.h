#ifndef _PRINT_H_
#define _PRINT_H_ 1

#include "util.h"
#include "gc.h"
#include "buildin.h"

#define toStr(v) _Generic(v,                        \
                          array *                   \
                          : arrayStr((array *)(v)), \
                            slice *                 \
                          : sliceStr((slice *)(v)), \
                            map *                   \
                          : mapStr((map *)(v)))

str *arrayStr(array *a);
str *sliceStr(slice *a);
str *mapStr(map *m);

#endif