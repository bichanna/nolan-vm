#ifndef GC_H
#define GC_H

#include "./darr.h"
#include "./value.h"

typedef struct GC GC;

struct GC {
  DArr *objects;  // list of Obj
  DArr *stack;    // list of Val
  size_t max;
};

GC *gc_create(size_t max, DArr *stack);

Val new_int(int64_t integer);
Val new_float(double float_num);
Val new_bool(bool boolean);
Val new_void();
Val new_str(GC *gc, char *chars);
Val new_list(GC *gc, DArr *list);

void may_perform_gc(GC *gc);
void perform_gc(GC *gc);

#endif
