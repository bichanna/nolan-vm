#ifndef VM_H
#define VM_H

#include <cstdint>
#include <vector>
#include "./value.hpp"

#define INITIAL_STACK_SIZE 512

namespace vm {
  using namespace std;

  enum class OpCode : uint8_t {
    NoOp = 0,
    LoadConst,
    INegate,
    FNegate,
    BNegate,
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
    InitStr,
    Jump,
    LJump,
    JumpIfFalse,
    Eq,
    NEq,
    GT,
    LT,
    GTEq,
    LTEq,
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
    vector<string*> run();
  
  private:
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
