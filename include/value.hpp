#ifndef VALUE_H
#define VALUE_H

#include <cstdint>
#include <string>
#include <vector>

namespace gc {
struct Val;

enum class ObjType { Str, List, Func };
enum class ValType { Int, Float, Bool, Void, Obj };

class Obj {
 public:
  struct Func {
    Obj* name;
    std::vector<std::uint8_t>::iterator startingIp;
    std::uint8_t arity;
  };

  bool marked;
  ObjType objType;
  union {
    std::string* str;
    std::vector<Val>* list;
    Func func;
  };

  Obj(std::string* str);
  Obj(std::vector<Val>* list);
  Obj(Obj* name, std::vector<std::uint8_t>::iterator startingIp, std::uint8_t arity);
  void mark();
};

struct Val {
  ValType valType;
  union {
    Obj* obj;
    std::int64_t integer;
    double floatNum; /* I wish I could use std::float64_t here :( */
    bool boolean;
    char voidVal;
  };
};

}  // namespace gc

#endif /* VALUE_H */
