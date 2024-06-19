#include "../include/gc.hpp"
#include "../include/value.hpp"

using namespace gc;

GC::GC(int max_obj_num) {
  this->objs = {};
  this->max_obj_num = max_obj_num;
}

void GC::gc(std::vector<Val>* stack) {
  // Mark all
  for (auto value : *stack) {
    if (value.is_obj) {
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

  this->max_obj_num = this->objs.size() * 2;
}

Val GC::new_int(std::int64_t integer, std::vector<Val>* stack) {
  if (this->objs.size() == this->max_obj_num) this->gc(stack);

  return Val {
    .integer = integer,
    .is_obj = false
  };
}

Val GC::new_float(double float_num, std::vector<Val>* stack) {
  if (this->objs.size() == this->max_obj_num) this->gc(stack);
  
  return Val {
    .float_num = float_num,
    .is_obj = false
  };
}

Val GC::new_bool(bool value, std::vector<Val>* stack) {
  if (this->objs.size() == this->max_obj_num) this->gc(stack);
  
  return Val {
    .boolean = value,
    .is_obj = false
  };
}

Val GC::new_void(std::vector<Val>* stack) {
  if (this->objs.size() == this->max_obj_num) this->gc(stack);

  return Val {
    .void_t = 0,
    .is_obj = false
  };
}

Val GC::new_str(std::string* str, std::vector<Val>* stack) {
  if (this->objs.size() == this->max_obj_num) this->gc(stack);
  
  auto obj = new Obj(str);
  this->objs.push_back(obj);
  
  return Val {
    .obj = obj,
    .is_obj = true
  };
}

Val GC::new_list(std::vector<Val>* list, std::vector<Val>* stack) {
  if (this->objs.size() == this->max_obj_num) this->gc(stack);
  
  auto obj = new Obj(list);
  this->objs.push_back(obj);
  
  return Val {
    .obj = obj,
    .is_obj = true
  };
}
