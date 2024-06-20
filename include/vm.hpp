#ifndef VM_H
#define VM_H

#include "./gc.hpp"
#include "./value.hpp"
#include <cstdint>
#include <filesystem>
#include <vector>

#define INITIAL_STACK_SIZE 512
#define MAGIC_NUMBER 2006 + 2018 + 0422 + 0305

namespace vm {
using namespace std;
namespace fs = filesystem;

enum class OpCode : uint8_t {
  NoOp = 0,
  LoadConst,
  INeg,
  FNeg,
  BNeg,
  IAdd,
  FAdd,
  SAdd,
  ISub,
  FSub,
  IMul,
  FMul,
  FDiv,
  IMod,
  FMod,
  Pop,
  PopN,
  InitList,
  Jump,
  LJump,
  JumpIfFalse,
  BEq,
  IEq,
  FEq,
  StrEq,
  IGT,
  FGT,
  StrGT,
  ILT,
  FLT,
  StrLT,
  IGTEq,
  FGTEq,
  StrGTEq,
  ILTEq,
  FLTEq,
  StrLTEq,
  And,
  Or,
  ILoad0,
  ILoad1,
  ILoad2,
  ILoad3,
  FLoad0,
  FLoad1,
  FLoad2,
  FLoad3,
  LoadTrue,
  LoadFalse,
  LoadVoid,
  Halt = 255
};

enum class ConstByteCode : uint8_t {
  Str = 0,
  Int8,
  Int64,
  Float,
  Void,
  ConstEnd = 255,
};

struct Program {
  vector<gc::Val> *consts;
  vector<uint8_t> *instructions;

  Program();
};

class VM {
public:
  VM(fs::path filePath);
  ~VM();
  vector<string *> *run();

private:
  gc::GC *gc;
  vector<uint8_t>::iterator ip;
  vector<gc::Val> stack;
  vector<gc::Val> *consts;
  vector<uint8_t> *instructions;
  vector<string *> errors;

  bool nextIp();

  uint8_t readByte();
  uint16_t readTwoBytes();
  void push(gc::Val val);
  gc::Val pop();
  void pop(int num);
  gc::Val peek(uint8_t peek);
  void jump(uint16_t jump);
  gc::Val readConst();
};

Program readByteCodes(fs::path filePath, gc::GC *gc, vector<gc::Val> &stack);
bool createStr(string &str, vector<byte>::iterator &bi,
               vector<byte>::iterator end);
bool createInt8(int64_t &integer, vector<byte>::iterator &bi,
                vector<byte>::iterator end);
bool createInt64(int64_t &integer, vector<byte>::iterator &bi,
                 vector<byte>::iterator end);
bool createFloat(double &floatNum, vector<byte>::iterator &bi,
                 vector<byte>::iterator end);
} // namespace vm

#endif /* VM_H */
