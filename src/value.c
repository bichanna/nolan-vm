#include "./value.h"
#include "./utf8.h"

#include <stdlib.h>
#include <string.h>

Obj *create_objstr_from(char *chars) {
  int len = strlen(chars) + 1;
  
  Obj *obj = malloc(sizeof(Obj));
  if (obj == NULL) return NULL;

  obj->marked = false;
  obj->obj_t = OBJ_STR;

  ObjStr *str = malloc(sizeof(ObjStr));
  if (str == NULL) return NULL;
  obj->str = str;
  
  str->chars = chars;
  str->bytes_length = len - 1;
  str->length = utf8_chars_length(chars);

  return obj;
}

Obj *create_objlist_from(Val *values, size_t len) {
  Obj *obj = malloc(sizeof(Obj));
  if (obj == NULL) return NULL;

  obj->marked = false;
  obj->obj_t = OBJ_LIST;

  DArr *list = darr_create(sizeof(Val));
  if (list == NULL) return NULL;
  
  darr_set_capacity(list, len);
  for (int i = 0; i < len; i++) darr_push(list, &values[0]);

  obj->list = list;
  
  return obj;
}

void free_val(Val val) {
  switch (val.val_t) {
    case VAL_OBJ:
      free_obj(val.object);
      break;
    default:
      break;
  }
}

void free_obj(Obj* obj) {
  switch (obj->obj_t) {
    case OBJ_STR: {
      free(obj->str->chars);
      free(obj->str);
      break;
    }
    case OBJ_LIST: {
      for (int i = 0; i < obj->list->count; i++) free_val(*(Val*)darr_get(obj->list, i));
      darr_destroy(obj->list);
      break;
    }
  }
  
  free(obj);
}
