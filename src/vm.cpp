#include "../include/vm.hpp"
#include "../include/util.hpp"

#include <cmath>

using namespace vm;

VM::VM(std::vector<gc::Val>* consts, std::vector<uint8_t>* instructions) {
  this->consts = consts;
  this->instructions = instructions;
  
  this->stack = {};
  this->stack.reserve(INITIAL_STACK_SIZE);
  
  this->errors = {};

  this->ip = this->instructions->begin();

  this->gc = new gc::GC(24);
}

VM::~VM() {
  delete this->gc;
  this->gc = nullptr;
  
  delete this->consts;
  this->consts = nullptr;
  
  delete this->instructions;
  this->instructions = nullptr;
}

std::vector<string*>* VM::run() {
  using namespace vm;

  bool halt = false;

  do {
    OpCode opcode = static_cast<OpCode>(*(this->ip));
    switch (opcode) {
      case OpCode::NoOp:
        break;
      case OpCode::LoadConst:
        this->push(this->readConst());
        break;
      case OpCode::INeg:
        this->push(this->gc->newInt(-(this->pop().integer), &this->stack));
        break;
      case OpCode::FNeg:
        this->push(this->gc->newFloat(-(this->pop().floatNum), &this->stack));
        break;
      case OpCode::BNeg:
        this->push(this->gc->newBool(!(this->pop().boolean), &this->stack));
        break;
      case OpCode::IAdd:
        this->push(this->gc->newInt(this->pop().integer + this->pop().integer, &this->stack));
        break;
      case OpCode::FAdd:
        this->push(this->gc->newFloat(this->pop().floatNum + this->pop().floatNum, &this->stack));
        break;
      case OpCode::ISub:
        this->push(this->gc->newInt(this->pop().integer - this->pop().integer, &this->stack));
        break;
      case OpCode::FSub:
        this->push(this->gc->newFloat(this->pop().floatNum - this->pop().floatNum, &this->stack));
        break;
      case OpCode::IMul:
        this->push(this->gc->newInt(this->pop().integer * this->pop().integer, &this->stack));
        break;
      case OpCode::FMul:
        this->push(this->gc->newFloat(this->pop().floatNum * this->pop().floatNum, &this->stack));
        break;
      case OpCode::FDiv:
        this->push(this->gc->newFloat(this->pop().floatNum / this->pop().floatNum, &this->stack));
        break;
      case OpCode::IMod:
        this->push(this->gc->newInt(this->pop().integer % this->pop().integer, &this->stack));
        break;
      case OpCode::FMod:
        this->push(this->gc->newFloat(fmod(this->pop().floatNum, this->pop().floatNum), &this->stack));
        break;
      case OpCode::Pop:
        this->pop();
        break;
      case OpCode::PopN:
        this->pop(this->readByte());
        break;
      case OpCode::InitList:
        {
          int length = this->readByte();
          std::vector<gc::Val>* list = new std::vector<gc::Val>();
          list->reserve(length);
          for (int i = 0; i < length; i++)
            list->push_back(this->readConst());
          this->push(this->gc->newList(list, &this->stack));
          break;
        }
      case OpCode::Jump:
        this->jump(this->readByte());
        break;
      case OpCode::LJump:
        this->jump(this->readTwoBytes());
        break;
      case OpCode::JumpIfFalse:
        if (!this->pop().boolean)
          this->jump(this->readTwoBytes());
        break;
      case OpCode::BEq:
        this->push(this->gc->newBool(this->pop().boolean == this->pop().boolean, &this->stack));
        break;
      case OpCode::IEq:
        this->push(this->gc->newBool(this->pop().integer == this->pop().integer, &this->stack));
        break;
      case OpCode::FEq:
        this->push(this->gc->newBool(this->pop().floatNum == this->pop().floatNum, &this->stack));
        break;
      case OpCode::StrEq:
        this->push(this->gc->newBool(*(this->pop().obj->str) == *(this->pop().obj->str), &this->stack));
        break;
      case OpCode::IGT:
        this->push(this->gc->newBool(this->pop().integer > this->pop().integer, &this->stack));
        break;
      case OpCode::FGT:
        this->push(this->gc->newBool(this->pop().floatNum > this->pop().floatNum, &this->stack));
        break;
      case OpCode::StrGT:
        this->push(this->gc->newBool((this->pop().obj->str->compare(*this->pop().obj->str) > 0), &this->stack));
        break;
      case OpCode::ILT:
        this->push(this->gc->newBool(this->pop().integer < this->pop().integer, &this->stack));
        break;
      case OpCode::FLT:
        this->push(this->gc->newBool(this->pop().floatNum < this->pop().floatNum, &this->stack));
        break;
      case OpCode::StrLT:
        this->push(this->gc->newBool((this->pop().obj->str->compare(*this->pop().obj->str) < 0), &this->stack));
        break;
      case OpCode::IGTEq:
        this->push(this->gc->newBool(this->pop().integer > this->pop().integer, &this->stack));
        break;
      case OpCode::FGTEq:
        this->push(this->gc->newBool(this->pop().floatNum > this->pop().floatNum, &this->stack));
        break;
      case OpCode::StrGTEq:
        this->push(this->gc->newBool((this->pop().obj->str->compare(*this->pop().obj->str) >= 0), &this->stack));
        break;
      case OpCode::ILTEq:
        this->push(this->gc->newBool(this->pop().integer < this->pop().integer, &this->stack));
        break;
      case OpCode::FLTEq:
        this->push(this->gc->newBool(this->pop().floatNum < this->pop().floatNum, &this->stack));
        break;
      case OpCode::StrLTEq:
        this->push(this->gc->newBool((this->pop().obj->str->compare(*this->pop().obj->str) <= 0), &this->stack));
        break;
      case OpCode::And:
        this->push(this->gc->newBool(this->pop().boolean && this->pop().boolean, &this->stack));
        break;
      case OpCode::Or:
        this->push(this->gc->newBool(this->pop().boolean || this->pop().boolean, &this->stack));
        break;
      case OpCode::ILoad0:
        this->push(this->gc->newInt(0, &this->stack));
        break;
      case OpCode::ILoad1:
        this->push(this->gc->newInt(1, &this->stack));
        break;
      case OpCode::ILoad2:
        this->push(this->gc->newInt(2, &this->stack));
        break;
      case OpCode::ILoad3:
        this->push(this->gc->newInt(3, &this->stack));
        break;
      case OpCode::FLoad0:
        this->push(this->gc->newFloat(0.0, &this->stack));
        break;
      case OpCode::FLoad1:
        this->push(this->gc->newFloat(1.0, &this->stack));
        break;
      case OpCode::FLoad2:
        this->push(this->gc->newFloat(2.0, &this->stack));
        break;
      case OpCode::FLoad3:
        this->push(this->gc->newFloat(3.0, &this->stack));
        break;
      case OpCode::LoadTrue:
        this->push(this->gc->newBool(true, &this->stack));
        break;
      case OpCode::LoadFalse:
        this->push(this->gc->newBool(false, &this->stack));
        break;
      case OpCode::LoadVoid:
        this->push(this->gc->newVoid(&this->stack));
        break;
      case OpCode::Halt:
        halt = true;
        break;
      default:
        return &this->errors;
    }
  } while (this->nextIp() && (!halt));

  return &this->errors;
}

bool VM::nextIp() {
  this->ip = this->ip + 1;
  return this->ip != this->instructions->end();
}

std::uint8_t VM::readByte() {
  this->nextIp();
  return *(this->ip);
}

std::uint16_t VM::readTwoBytes() {
  return util::fromLittleEndian({this->readByte(), this->readByte()});
}

void VM::push(gc::Val val) {
  this->stack.push_back(val);
}

gc::Val VM::pop() {
  gc::Val last = this->stack.back();
  this->stack.pop_back();
  return last;
}

void VM::pop(int num) {
  for (int i = 0; i < num; i++) {
    this->pop();
  }
}

gc::Val VM::peek(uint8_t peek) {
  return this->stack.at(this->stack.size() - 1 - peek);
}

void VM::jump(uint16_t jump) {
  this->ip = this->ip + jump;
}

gc::Val VM::readConst() {
  return this->consts->at(this->readTwoBytes());
}
