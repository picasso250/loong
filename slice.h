#ifndef _SLICE_H_
#define _SLICE_H_ 1

#include "util.h"
#include "gc.h"
#include "type.h"

/*
 * types are checked dynamically
 * make macro simple
 * do not care about GC
 */

array *_initarray(array *arr, char *typeStr, int len, int elemSize);
#define initarray(a, T, len) _initarray(a, #T, len, sizeof(T))
#define newarray(T, len) _initarray(alloc(array), #T, len, sizeof(T))

slice *_initslice(slice *v, char *typeStr, int elemSize, int len, int cap);
#define initslice(v, T, len, cap) _initslice(v, #T, sizeof(T), len, cap)
#define newslice(T, len, cap) _initslice(alloc(slice), #T, sizeof(T), len, len)

#define cap(v) ((v)->array->len)

void *_push(char *type, slice *v, int elemSize);
#define push(v, T, a) \
  (*(T *)_push(#T, (v), sizeof(a)) = a)
int _pop(slice *v, char *typeStr, void *p, int elemSize);
#define pop(v, T, a) _pop((v), #T, &(a), sizeof(a))

void *_getarray_(char *type, array *v, int i, int elemSize);
#define _getarray(v, i, T) (*(T *)_getarray_(#T, (array *)v, (int)i, sizeof(T)))
void *_setarray_(char *type, array *v, int i, int elemSize);
#define _setarray(v, i, T, a) \
  (*(T *)_setarray_(#T, (v), (int)i, sizeof(a)) = a)

void *_sliceget_(char *type, slice *v, int i, int elemSize);
#define _getslice(v, i, T) (*(T *)_sliceget_(#T, (slice *)v, (int)i, sizeof(T)))
void *_setslice_(char *type, slice *v, int i, int elemSize);
#define _setslice(v, i, T, a) \
  (*(T *)_setslice_(#T, (v), (int)i, sizeof(a)) = a)

slice sl(slice *v, int start, int end);
slice *slp(slice *v, int start, int end);

#define copy(dst, i, j, src, k, l) _copy(dst, i, j, src, k, l, dst->array->elemSize, src->array->elemSize)

void setArrayCap(array *a, int newCap);

#define forslice(s, i, T, v)       \
  for (int i = 0; i < s->len; i++) \
  {                                \
    T v = *((T *)s->start + i);
#define endforslcie }

#endif