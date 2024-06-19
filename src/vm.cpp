#include "../include/vm.hpp"
#include "../include/util.hpp"

using namespace vm;

VM::VM(std::vector<gc::Val>* consts, std::vector<uint8_t>* instructions) {
  this->consts = consts;
  this->instructions = instructions;
  
  this->stack = {};
  this->stack.reserve(INITIAL_STACK_SIZE);
  
  this->errors = {};

  this->ip = this->instructions->begin();
}

std::vector<string*> VM::run() {
  /* TODO: implement this */
  return this->errors;
}

bool VM::next_ip() {
  this->ip = this->ip + 1;
  return this->ip != this->instructions->end();
}

std::uint8_t VM::read_byte() {
  this->next_ip();
  return *(this->ip);
}

std::uint16_t VM::read_two_bytes() {
  return util::from_little_endian({this->read_byte(), this->read_byte()});
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

gc::Val VM::read_const() {
  return this->consts->at(this->read_two_bytes());
}
