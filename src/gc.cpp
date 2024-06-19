#include "../include/gc.hpp"
#include "../include/value.hpp"

using namespace gc;

GC::GC(int maxObjNum) {
  this->objs = {};
  this->maxObjNum = maxObjNum;
}

void GC::gc(std::vector<Val>* stack) {
  // Mark all
  for (auto value : *stack) {
    if (value.isObj) {
      value.obj->mark();
    }
  }

  // Sweep
  for (auto obj : this->objs) {
    if (!obj->marked) {
      // Remove it from the list from the list and free it.
      this->objs.remove(obj);
      delete obj;
    } else {
      // Unmark it and move on.
      obj->marked = false;
    }
  }

  this->maxObjNum = this->objs.size() * 2;
}

Val GC::newInt(std::int64_t integer, std::vector<Val>* stack) {
  if (this->objs.size() == this->maxObjNum) this->gc(stack);

  return Val {
    .integer = integer,
    .isObj = false
  };
}

Val GC::newFloat(double floatNum, std::vector<Val>* stack) {
  if (this->objs.size() == this->maxObjNum) this->gc(stack);
  
  return Val {
    .floatNum = floatNum,
    .isObj = false
  };
}

Val GC::newBool(bool value, std::vector<Val>* stack) {
  if (this->objs.size() == this->maxObjNum) this->gc(stack);
  
  return Val {
    .boolean = value,
    .isObj = false
  };
}

Val GC::newVoid(std::vector<Val>* stack) {
  if (this->objs.size() == this->maxObjNum) this->gc(stack);

  return Val {
    .voidVal = 0,
    .isObj = false
  };
}

Val GC::newStr(std::string* str, std::vector<Val>* stack) {
  if (this->objs.size() == this->maxObjNum) this->gc(stack);
  
  auto obj = new Obj(str);
  this->objs.push_back(obj);
  
  return Val {
    .obj = obj,
    .isObj = true
  };
}

Val GC::newList(std::vector<Val>* list, std::vector<Val>* stack) {
  if (this->objs.size() == this->maxObjNum) this->gc(stack);
  
  auto obj = new Obj(list);
  this->objs.push_back(obj);
  
  return Val {
    .obj = obj,
    .isObj = true
  };
}
