#include "../include/value.hpp"

using namespace gc;

Obj::Obj(std::string* str) {
  this->str = str;
  this->objType = ObjType::String;
  this->marked = false;
}

Obj::Obj(std::vector<Val>* list) {
  this->list = list;
  this->objType = ObjType::List;
  this->marked = false;
}

void Obj::mark() {
  if (this->marked) return;

  this->marked = true;

  if (this->objType == ObjType::List) {
    for (auto elem : *(this->list)) {
      if (elem.isObj) {
        elem.obj->mark();
      }
    }
  }
}
