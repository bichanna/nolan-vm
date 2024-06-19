#ifndef GC_H
#define GC_H

#include "./value.hpp"
#include <list>

namespace gc {
  class GC {
  public:
    GC(int max_obj_num);

    void gc(std::vector<Val>* values);
    
    Val new_int(std::int64_t integer, std::vector<Val>* stack);
    Val new_float(double float_num, std::vector<Val>* stack);
    Val new_bool(bool value, std::vector<Val>* stack);
    Val new_void(std::vector<Val>* stack);
    Val new_str(std::string* str, std::vector<Val>* stack);
    Val new_list(std::vector<Val>* list, std::vector<Val>* stack);

  private:
    std::list<Obj*> objs;
    int max_obj_num;
  };
}

#endif /* GC_H */
