#ifndef VM_H
#define VM_H

#include <cstdint>
#include <vector>
#include "./value.hpp"

#define INITIAL_STACK_SIZE 256

namespace vm {
  using namespace std;

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

    bool next_ip();

    uint8_t read_byte();
    uint16_t read_two_bytes();
    void push(gc::Val val);
    gc::Val pop();
    void pop(int num);
    gc::Val peek(uint8_t peek);
    void jump(uint16_t jump);
    gc::Val read_const();
  };
}

#endif /* VM_H */
