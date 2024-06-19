#ifndef VM_H
#define VM_H

#include <cstdint>
#include <vector>
#include "./value.hpp"
#include "gc.hpp"

#define INITIAL_STACK_SIZE 512

namespace vm {
  using namespace std;

  enum class OpCode : uint8_t {
    NoOp = 0,
    LoadConst,
    INeg,
    FNeg,
    BNeg,
    IAdd,
    FAdd,
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

  class VM {
  public:
    VM(vector<gc::Val>* consts, vector<uint8_t>* instructions);
    ~VM();
    vector<string*>* run();
  
  private:
    gc::GC* gc;
    vector<uint8_t>::iterator ip;
    vector<gc::Val> stack;
    vector<gc::Val>* consts;
    vector<uint8_t>* instructions;
    vector<string*> errors;

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
}

#endif /* VM_H */
