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

Val GC::newInt(std::int64_t integer) {
  Val v{};
  v.valType = gc::ValType::Int;
  v.integer = integer;
  return v;
}

Val GC::newFloat(double floatNum) {
  Val v{};
  v.valType = gc::ValType::Float;
  v.floatNum = floatNum;
  return v;
}

Val GC::newBool(bool value) {
  Val v{};
  v.valType = gc::ValType::Bool;
  v.boolean = value;
  return v;
}

Val GC::newVoid() {
  Val v{};
  v.valType = gc::ValType::Void;
  v.voidVal = 0;
  return v;
}

Val GC::newStr(std::string* str, std::vector<Val>& stack) {
  this->mayPerformGC(stack);

  auto obj = new Obj(str);
  this->objs.push_back(obj);

  Val v{};
  v.valType = gc::ValType::Obj;
  v.obj = obj;

  return v;
}

Val GC::newList(std::vector<Val>* list, std::vector<Val>& stack) {
  this->mayPerformGC(stack);

  auto obj = new Obj(list);
  this->objs.push_back(obj);

  Val v{};
  v.valType = gc::ValType::Obj;
  v.obj = obj;

  return v;
}

Val GC::newFunc(Obj* name, std::vector<std::uint8_t>::iterator startingIp, std::uint8_t arity,
                std::vector<Val>& stack) {
  this->mayPerformGC(stack);

  auto obj = new Obj(name, startingIp, arity);
  this->objs.push_back(obj);

  Val v{};
  v.valType = gc::ValType::Obj;
  v.obj = obj;

  return v;
}

void GC::mayPerformGC(std::vector<Val>& stack) {
  if (this->objs.size() == static_cast<size_t>(this->maxObjNum)) this->gc(stack);
}
