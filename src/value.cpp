#include "../include/value.hpp"

using namespace gc;

Obj::Obj(std::string* str) {
  this->str = str;
  this->obj_t = ObjType::String;
  this->marked = false;
}

Obj::Obj(std::vector<Val>* list) {
  this->list = list;
  this->obj_t = ObjType::List;
  this->marked = false;
}

void Obj::mark() {
  if (this->marked) return;

  this->marked = true;

  if (this->obj_t == ObjType::List) {
    for (auto elem : *(this->list)) {
      if (elem.is_obj) {
        elem.obj->mark();
      }
    }
  }
}
