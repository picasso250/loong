#ifndef _TYPE_H_
#define _TYPE_H_ 1

#include <stdbool.h>

#include "util.h"

typedef unsigned GCFlag;

typedef unsigned char byte; // we only use int,float,byte

typedef struct array
{
  GCFlag flag;
  char *type; // should be compile time checked
  int cap;
  int elemSize;
  char *base; // T[] not T*[]
} array;
// slice can be a value type, not a ref type
typedef struct slice
{
  GCFlag flag;
  array *array;
  int len;
  int offset;
} slice;

typedef struct map
{
    GCFlag flag;
    const char *type;
    int elemSize;
    int len;
    unsigned factor;
    struct mapentry *first;
    struct mapentry **base;
} map;
typedef struct mapentry
{
    char *key;
    void *value;
    struct mapentry *next;
} mapentry;

#define INT 253914290
#define FLOAT 437721913
#define BYTE 687585745
#define BOOL 359348725

bool isPrimitive(char *typeStr);

#endif