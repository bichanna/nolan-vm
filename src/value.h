#ifndef VALUE_H
#define VALUE_H

#include <stdint.h>
#include <string.h>

#include "./darr.h"

typedef enum { OBJ_STR, OBJ_LIST } ObjType;

typedef enum { VAL_INT, VAL_FLOAT, VAL_BOOL, VAL_VOID, VAL_OBJ } ValType;

typedef struct Val Val;
typedef struct Obj Obj;
typedef struct ObjStr ObjStr;
typedef struct ObjList ObjList;

struct Val {
  union {
    int64_t integer;
    double float_num;
    Obj* object;
    bool boolean;
  };
  ValType val_t;
};

struct Obj {
  bool marked;
  ObjType obj_t;
  union {
    ObjStr* str;
    DArr* list;  // List of Val
  };
};

struct ObjStr {
  char* chars;
  size_t length;
  size_t bytes_length;
};

Obj *create_objstr_from(char* chars);
Obj *create_objlist_from(Val* values, size_t len);

void free_val(Val val);
void free_obj(Obj* obj);

#endif
