#ifndef GC_H
#define GC_H

#include "./value.hpp"
#include <list>

namespace gc {
  class GC {
  public:
    GC(int maxObjNum);

    void gc(std::vector<Val>* values);
    
    Val newInt(std::int64_t integer, std::vector<Val>* stack);
    Val newFloat(double floatNum, std::vector<Val>* stack);
    Val newBool(bool value, std::vector<Val>* stack);
    Val newVoid(std::vector<Val>* stack);
    Val newStr(std::string* str, std::vector<Val>* stack);
    Val newList(std::vector<Val>* list, std::vector<Val>* stack);

  private:
    std::list<Obj*> objs;
    int maxObjNum;
  };
}

#endif /* GC_H */
