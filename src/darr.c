#include "./darr.h"

#include <string.h>

DArr *darr_create(size_t t_size) {
  DArr *da = (DArr *)malloc(sizeof(DArr));
  if (da == NULL) return NULL;

  da->count = 0;
  da->capacity = DARR_INIT_SIZE;
  da->t_size = t_size;

  da->data = malloc(DARR_INIT_SIZE * t_size);
  if (da == NULL) {
    free(da);
    return NULL;
  }

  return da;
}

bool darr_push(DArr *da, void *elem) {
  if (da->count == da->capacity) {
    da->capacity *= 2;
    void *new_data = realloc(da->data, da->capacity * da->t_size);
    if (new_data == NULL) return false;
  }

  void *dest = (char *)da->data + (da->count * da->t_size);
  memcpy(dest, elem, da->t_size);

  da->count++;

  return true;
}

void *darr_pop(DArr *da) {
  if (da->count == 0) return NULL;
  return &(da->data[--(da->count)]);
}

void *darr_get(DArr *da, size_t idx) {
  if (da->count <= idx) return NULL;
  return &(da->data[idx]);
}

void darr_set(DArr *da, size_t idx, void *val) {
  if (da->count <= idx) return;
  void *dest = (char *)da->data + ((idx + 1) * da->t_size);
  memcpy(dest, val, da->t_size);
}

void darr_set_capacity(DArr *da, size_t new_cap) {
  void *new_data = realloc(da->data, da->t_size * new_cap);
  da->data = new_data;
  da->capacity = new_cap;
}

void darr_destroy(DArr *da) {
  free(da->data);
  free(da);
}
