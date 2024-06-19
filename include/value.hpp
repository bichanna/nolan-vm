#ifndef VALUE_H
#define VALUE_H

#include <cstdint>
#include <string>
#include <vector>

namespace gc {
  struct Val;

  enum class ObjType {
    String,
    List
  };

  class Obj {
  public:
    bool marked;
    ObjType objType;
    union {
      std::string* str;
      std::vector<Val>* list;
    };

    Obj(std::string* str);
    Obj(std::vector<Val>* list);
    void mark();
  };
  
  struct Val {
    union {
      Obj* obj;
      std::int64_t integer;
      double floatNum; /* I wish I could use std::float64_t here :( */
      bool boolean;
      char voidVal;
    };
    bool isObj;
  };

}

#endif /* VALUE_H */
