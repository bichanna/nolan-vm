#include "../include/value.hpp"

using namespace gc;

Obj::Obj(std::string* str) {
  this->str = str;
  this->objType = ObjType::Str;
  this->marked = false;
}

Obj::Obj(std::vector<Val>* list) {
  this->list = list;
  this->objType = ObjType::List;
  this->marked = false;
}

Obj::Obj(Obj* name, std::vector<std::uint8_t>::iterator startingIp, std::uint8_t arity) {
  this->func = {name, startingIp, arity};
  this->objType = ObjType::Func;
  this->marked = false;
}

void Obj::mark() {
  if (this->marked) return;

  this->marked = true;

  switch (this->objType) {
    case ObjType::List:
      for (auto elem : *(this->list))
        if (elem.valType == gc::ValType::Obj) elem.obj->mark();
      break;
    case ObjType::Func:
      this->func.name->mark();
      break;
    default:
      break;
  }
}
