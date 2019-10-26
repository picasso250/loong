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

void *_push(char *type, slice *v);
#define push(v, T, a) \
  (*(T *)_push(#T, (v)) = (T)(a))
int _pop(slice *v, char *typeStr, void *p);
#define pop(v, T, a) _pop((v), #T, &(a))

void *_getarray_(char *type, array *v, int i);
#define _getarrayp(v, i, T) \
  (_getarray_(#T, (array *)(v), (int)i))
void *_setarray_(char *type, array *v, int i);
#define _setarray(v, i, T, a) \
  (*(T *)_setarray_(#T, (v), (int)(i)) = (T)(a))

void *_sliceget_(char *type, slice *v, int i);
#define _getslicep(v, i, T) \
  ((T *)_sliceget_(#T, (slice *)(v), (int)(i)))
#define _getslice(v, i, T) \
  (*_getslicep(v, i, T))
void *_setslice_(char *type, slice *v, int i);
#define _setslice(v, i, T, a) \
  (*(T *)_setslice_(#T, (v), (int)(i)) = (T)(a))

slice sl(slice *v, int start, int end);
slice *slp(slice *v, int start, int end);

void _copy(slice *dst, int i, int ii, slice *src, int k, int kk);
#define copy(dst, i, j, src, k, l) _copy(dst, i, j, src, k, l)

void setArrayCap(array *a, int newCap);

#define forslice(s, i, T, v)       \
  for (int i = 0; i < (s)->len; i++) \
  {                                \
    T v = *((T *)(s)->array->base + s->offset + i);
#define endforslcie }

#endif