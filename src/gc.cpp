#include "../include/gc.hpp"

#include "../include/value.hpp"

using namespace gc;

GC::GC(int maxObjNum) {
  this->objs = std::list<Obj*>();
  this->maxObjNum = maxObjNum;
}

void GC::gc(std::vector<Val>& stack) {
  // Mark all
  for (auto value : stack) {
    if (value.valType == gc::ValType::Obj) {
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

Val GC::newInt(std::int64_t integer) { return {.integer = integer, .valType = gc::ValType::Int}; }

Val GC::newFloat(double floatNum) { return {.floatNum = floatNum, .valType = gc::ValType::Float}; }

Val GC::newBool(bool value) { return {.boolean = value, .valType = gc::ValType::Bool}; }

Val GC::newVoid() { return {.voidVal = 0, .valType = gc::ValType::Void}; }

Val GC::newStr(std::string* str, std::vector<Val>& stack) {
  this->mayPerformGC(stack);

  auto obj = new Obj(str);
  this->objs.push_back(obj);

  return {.obj = obj, .valType = gc::ValType::Obj};
}

Val GC::newList(std::vector<Val>* list, std::vector<Val>& stack) {
  this->mayPerformGC(stack);

  auto obj = new Obj(list);
  this->objs.push_back(obj);

  return {.obj = obj, .valType = gc::ValType::Obj};
}

void GC::mayPerformGC(std::vector<Val>& stack) {
  if (this->objs.size() == static_cast<size_t>(this->maxObjNum)) this->gc(stack);
}
