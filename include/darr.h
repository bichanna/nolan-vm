#ifndef DARR_H
#define DARR_H

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#define DARR_INIT_SIZE 64

#define MAKE_DARR(TYPE)                                    \
  typedef struct {                                         \
    TYPE *array;                                           \
    size_t count;                                          \
    size_t capacity;                                       \
  } DArr_##TYPE;                                           \
  DArr_##TYPE darr_##TYPE_create() {                       \
    DArr_##TYPE *da = malloc(sizeof(DArr_##TYPE));         \
    da->count = 0;                                         \
    da->capacity = DARR_INIT_SIZE;                         \
    da->array = malloc(DARR_INIT_SIZE * sizeof(TYPE));     \
  }                                                        \
  void darr_##TYPE_push(DArr_##TYPE *da, TYPE elem) {      \
    if (++(da->count) == da->capacity) {                   \
      da->capacity *= 2;                                   \
      da->array = realloc(da->array, da->capacity * TYPE); \
    }                                                      \
    da->array[da->count - 1] = elem;                       \
  }                                                        \
  ##TYPE *darr_##TYPE_pop(DArr_##TYPE *da) {               \
    if (da->count == 0) return NULL;                       \
    return &(da->array[--(da->count)]);                    \
  }                                                        \
  ##TYPE *darr_##TYPE_get(DArr_##TYPE *da, size_t idx) {   \
    if (da->count <= idx) return NULL;                     \
    return &(da->array[idx]);                              \
  }                                                        \
  void darr_##TYPE_destroy(DArr_##TYPE *da) {              \
    free(da->array);                                       \
    free(da);                                              \
  }

#endif
