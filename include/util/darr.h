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

#define MAKE_DARR(TYPE)                                             \
  typedef struct {                                                  \
    TYPE *array;                                                    \
    size_t count;                                                   \
    size_t capacity;                                                \
  } DArr_##TYPE;                                                    \
  DArr_##TYPE *darr_##TYPE_create() {                               \
    DArr_##TYPE *da = (DArr_##Type)malloc(sizeof(DArr_##TYPE));     \
    if (da == NULL) return NULL;                                    \
    da->count = 0;                                                  \
    da->capacity = DARR_INIT_SIZE;                                  \
    da->array = (##Type *)malloc(DARR_INIT_SIZE * sizeof(TYPE));    \
    if (da->array == NULL) {                                        \
      free(da);                                                     \
      return NULL;                                                  \
    }                                                               \
  }                                                                 \
  void darr_##TYPE_push(DArr_##TYPE *da, TYPE elem) {               \
    if (++(da->count) == da->capacity) {                            \
      da->capacity *= 2;                                            \
      da->array = realloc(da->array, da->capacity * TYPE);          \
    }                                                               \
    da->array[da->count - 1] = elem;                                \
  }                                                                 \
  ##TYPE *darr_##TYPE_pop(DArr_##TYPE *da) {                        \
    if (da->count == 0) return NULL;                                \
    return &(da->array[--(da->count)]);                             \
  }                                                                 \
  ##TYPE *darr_##TYPE_get(DArr_##TYPE *da, size_t idx) {            \
    if (da->count <= idx) return NULL;                              \
    return &(da->array[idx]);                                       \
  }                                                                 \
  void darr_##TYPE_set(DArr_##TYPE *da, size_t idx, ##TYPE value) { \
    if (da->count <= idx) return;                                   \
    da->array[idx] = value;                                         \
  }                                                                 \
  void darr_##TYPE_destroy(DArr_##TYPE *da) {                       \
    free(da->array);                                                \
    free(da);                                                       \
  }

#endif
