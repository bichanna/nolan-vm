#ifndef GC_H
#define GC_H

#include "./value.hpp"
#include <list>

namespace gc {
  class GC {
  public:
    GC(int maxObjNum);

    void gc(std::vector<Val>& stack);
    
    Val newInt(std::int64_t integer);
    Val newFloat(double floatNum);
    Val newBool(bool value);
    Val newVoid();
    Val newStr(std::string* str, std::vector<Val>& stack);
    Val newList(std::vector<Val>* list, std::vector<Val>& stack);

  private:
    std::list<Obj*> objs;
    int maxObjNum;

    void mayPerformGC(std::vector<Val>& stack);
  };
}

#endif /* GC_H */
