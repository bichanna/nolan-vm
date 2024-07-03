#ifndef DARR_H
#define DARR_H

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

// A very simple dynamic array implementation using a big macro.
//
// Usage:
//
// MAKE_DARR(int);
//
// DArr_int *da = darr_int_create();
// darr_int_push(da, 123);
// darr_int_push(da, 321);
//
// for (int i = 0; i < da->count; i++) {
//   printf("%d\n", darr_int_get(da, i));
// }
//
// darr_int_destroy(da);

#define DARR_INIT_SIZE 64

typedef struct {
  void *data;
  size_t count;
  size_t capacity;
  size_t t_size;
} DArr;

DArr *darr_create(size_t t_size);
bool darr_push(DArr *da, void *elem);
void *darr_pop(DArr *da);
void *darr_get(DArr *da, size_t idx);
void darr_set(DArr *da, size_t idx, void *val);
void darr_destroy(DArr *da);

#endif
