#include "./gc.h"

GC *gc_create(size_t max, DArr *stack) {
  GC *gc = (GC *)malloc(sizeof(GC));
  if (gc == NULL) return NULL;

  gc->objects = darr_create(sizeof(Obj));
  gc->stack = stack;
  gc->max = max;

  return gc;
}

Val new_int(int64_t integer) {
  Val val;
  val.integer = integer;
  val.val_t = VAL_INT;
  return val;
}

Val new_float(double float_num) {
  Val val;
  val.float_num = float_num;
  val.val_t = VAL_FLOAT;
  return val;
}

Val new_bool(bool boolean) {
  Val val;
  val.boolean = boolean;
  val.val_t = VAL_BOOL;
  return val;
}

Val new_void() {
  Val val;
  val.val_t = VAL_VOID;
  return val;
}

// Val new_str(GC *gc, char *chars) {
//   may_perform_gc(gc);
//
//   ObjStr str;
//   str.chars = chars;
//   str.length = 0;
//
//   Obj obj;
//   obj.marked = false;
//   obj.obj_t = OBJ_STR;
//   // obj.str =
// }
