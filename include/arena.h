#ifndef ARENA_H
#define ARENA_H

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// A very simple arena allocator implementation. Not memory aligned.
//
// Usage:
//
// Arena *arena = make_arena();
// char *text = (char *)arena_alloc(arena, sizeof(6));
// text = "Hello";
//
// printf(text);
//
// arena_destroy(arena);

typedef struct {
  uint8_t *memory;
  size_t size;
  size_t used;
} Arena;

Arena *arena_create(size_t capacity) {
  Arena *arena = (Arena *)malloc(sizeof(Arena));
  if (arena == NULL) return NULL;

  arena->size = capacity;
  arena->used = 0;

  arena->memory = (uint8_t *)malloc(arena->size);
  if (arena->memory == NULL) {
    free(arena);
    return NULL;
  }

  return arena;
}

void *arena_alloc(Arena *arena, size_t size) {
  if (arena->used + size > arena->size) return NULL;

  void *ptr = &arena->memory[arena->used];
  arena->used += size;

  // Zero new memory.
  memset(ptr, 0, size);

  return ptr;
}

void arena_empty(Arena *arena) { arena->used = 0; }

void arena_destroy(Arena *arena) {
  free(arena->memory);
  free(arena);
}

#endif
