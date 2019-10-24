#ifndef _SLICE_H_
#define _SLICE_H_ 1

#include "util.h"
#include "gc.h"
// #include "type.h"

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
  char *start;
} slice;

/*
 * types are checked dynamically
 * make macro simple
 * do not care about GC
 */

array *_initarray(array *arr, char *typeStr, int len, int elemSize);
#define initarray(a, T, len) _initarray(a, #T, len, sizeof(T))
#define newarray(T, len) _initarray(alloc(array), #T, len, sizeof(T))

#define initslice(v, T, len, cap) _initslice(v, #T, sizeof(T), len, cap)
#define newslice(T, len, cap) _initslice(alloc(slice), #T, sizeof(T), len, len)

#define cap(v) ((v)->array->cap)

void *_push(char *type, slice *v, int elemSize);
#define push(v, T, a) \
  (*(T *)_push(#T, (v), sizeof(a)) = a)
int _pop(slice *v, void *p, int elemSize);
#define pop(v, a) _pop((v), &(a), sizeof(a))

void _sliceget_(slice *v, int i, void *p, int elemSize);
#define _getslice(v, i, a) _sliceget_(v, i, &a, sizeof(a))
void *_setslice_(char *type, slice *v, int i, int elemSize);
#define _setslice(v, T, i, a) \
  (*(T *)_setslice_(#T, (v), i, sizeof(a)) = a)

slice sl(slice *v, int start, int end);
slice *slp(slice *v, int start, int end);

#define copy(dst, i, j, src, k, l) _copy(dst, i, j, src, k, l, dst->array->elemSize, src->array->elemSize)

#endif