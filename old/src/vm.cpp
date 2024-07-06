#include "../include/vm.hpp"

#include <cmath>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <ios>
#include <iostream>
#include <sstream>

#include "../include/util.hpp"

using namespace vm;

Program::Program() {
  this->consts = new std::vector<gc::Val>();
  this->instructions = new std::vector<uint8_t>();
}

Program vm::readByteCodes(fs::path filePath, gc::GC* gc, std::vector<gc::Val>& stack) {
  std::ifstream file{filePath, std::ios::binary};

  if (!file.is_open()) {
    std::cerr << "Error opening file: " << filePath << std::endl;
    exit(EXIT_FAILURE);
  }

  // Check the Magic number.
  uint32_t magic{};
  file.read(reinterpret_cast<char*>(&magic), sizeof(uint32_t));
  if (magic != MAGIC_NUMBER) {
    std::cerr << "Invalid magic number: 0x" << std::hex << magic << std::endl;
    exit(EXIT_FAILURE);
  }

  auto fileSize = fs::file_size(filePath);
  if (fileSize == 0) {
    std::cerr << "Empty file" << std::endl;
    exit(EXIT_FAILURE);
  }

  Program program{};

  std::vector<std::byte> bytes{fileSize};
  file.read(reinterpret_cast<char*>(bytes.data()), fileSize);

  // Get all the constants.
  auto bi = bytes.begin();
  while (true) {
    switch (static_cast<ConstByteCode>(*bi)) {
      case ConstByteCode::Str: {
        std::string* str = new std::string();
        if (!createStr(*str, bi, bytes.end())) {
          std::cerr << "Invalid string value: " << *str << std::endl;
          delete program.consts;
          delete program.instructions;
          exit(EXIT_FAILURE);
        }
        program.consts->push_back(gc->newStr(str, stack));
        break;
      }
      case ConstByteCode::Int8: {
        int64_t integer{};
        if (!createInt8(integer, bi, bytes.end())) {
          std::cerr << "Invalid 8-bit integer: " << integer << std::endl;
          delete program.consts;
          delete program.instructions;
          exit(EXIT_FAILURE);
        }
        program.consts->push_back(gc->newInt(integer));
        break;
      }
      case ConstByteCode::Int64: {
        int64_t integer{};
        if (!createInt64(integer, bi, bytes.end())) {
          std::cerr << "Invalid 64-bit integer: " << integer << std::endl;
          delete program.consts;
          delete program.instructions;
          exit(EXIT_FAILURE);
        }
        program.consts->push_back(gc->newInt(integer));
        break;
      }
      case ConstByteCode::Float: {
        double floatNum{};
        if (!createFloat(floatNum, bi, bytes.end())) {
          std::cerr << "Invalid float-point number: " << floatNum << std::endl;
          delete program.consts;
          delete program.instructions;
          exit(EXIT_FAILURE);
        }
        program.consts->push_back(gc->newFloat(floatNum));
        break;
      }
      case ConstByteCode::Void: {
        ++bi;
        program.consts->push_back(gc->newVoid());
        break;
      }
      case ConstByteCode::ConstEnd:
        ++bi;
        goto exitLoop;
    }

    bi++;
  }

exitLoop:

  // The opcodes.
  while (bi != bytes.end()) {
    program.instructions->push_back(static_cast<uint8_t>(*bi));
    bi++;
  }

  file.close();

  return program;
}

bool vm::createStr(std::string& str, std::vector<std::byte>::iterator& bi,
                   std::vector<std::byte>::iterator end) {
  if (++bi == end) return false;

  switch (static_cast<uint8_t>(*bi)) {
    case 1: {
      auto len = static_cast<uint8_t>(*bi);
      for (int i = 0; i < len; i++) {
        if (++bi == end) return false;
        str.push_back(static_cast<char>(*bi));
      }
      break;
    }
    case 2: {
      auto b1 = static_cast<uint8_t>(*bi);
      if (++bi == end) return false;
      auto b2 = static_cast<uint8_t>(*bi);
      uint16_t len = util::fromLittleEndian(tuple<uint8_t, uint8_t>{b1, b2});
      for (int i = 0; i < len; i++) {
        if (++bi == end) return false;
        str.push_back(static_cast<char>(*bi));
      }
      break;
    }
    default:
      return false;
  }

  return true;
}

bool vm::createInt8(int64_t& integer, std::vector<std::byte>::iterator& bi,
                    std::vector<std::byte>::iterator end) {
  if (++bi == end) return false;
  integer = static_cast<int64_t>(static_cast<uint8_t>(*bi));
  return true;
}

bool vm::createInt64(int64_t& integer, std::vector<std::byte>::iterator& bi,
                     std::vector<std::byte>::iterator end) {
  if (++bi == end) return false;

  uint8_t arr[8]{};
  arr[0] = static_cast<uint8_t>(*bi);
  if (++bi == end) return false;
  arr[1] = static_cast<int8_t>(*bi);
  if (++bi == end) return false;
  arr[2] = static_cast<uint8_t>(*bi);
  if (++bi == end) return false;
  arr[3] = static_cast<uint8_t>(*bi);
  if (++bi == end) return false;
  arr[4] = static_cast<uint8_t>(*bi);
  if (++bi == end) return false;
  arr[5] = static_cast<uint8_t>(*bi);
  if (++bi == end) return false;
  arr[6] = static_cast<uint8_t>(*bi);
  if (++bi == end) return false;
  arr[7] = static_cast<uint8_t>(*bi);

  integer = util::fromLittleEndian(arr);

  return true;
}

bool vm::createFloat(double& floatNum, std::vector<std::byte>::iterator& bi,
                     std::vector<std::byte>::iterator end) {
  if (++bi == end) return false;

  uint8_t arr[8]{};
  arr[0] = static_cast<uint8_t>(*bi);
  if (++bi == end) return false;
  arr[1] = static_cast<int8_t>(*bi);
  if (++bi == end) return false;
  arr[2] = static_cast<uint8_t>(*bi);
  if (++bi == end) return false;
  arr[3] = static_cast<uint8_t>(*bi);
  if (++bi == end) return false;
  arr[4] = static_cast<uint8_t>(*bi);
  if (++bi == end) return false;
  arr[5] = static_cast<uint8_t>(*bi);
  if (++bi == end) return false;
  arr[6] = static_cast<uint8_t>(*bi);
  if (++bi == end) return false;
  arr[7] = static_cast<uint8_t>(*bi);

  floatNum = util::fromLittleEndianToFloat(arr);

  return true;
}

VM::VM(fs::path filePath) {
  this->stack = {};
  this->stack.reserve(INITIAL_STACK_SIZE);

  this->gc = new gc::GC(24);

  auto program = readByteCodes(filePath, this->gc, this->stack);
  this->consts = program.consts;
  this->instructions = program.instructions;

  this->globals = {};
}

VM::~VM() {
  delete this->gc;
  this->gc = nullptr;

  delete this->consts;
  this->consts = nullptr;

  delete this->instructions;
  this->instructions = nullptr;
}

std::string VM::run() {
  using namespace vm;

  bool halt = false;
  std::stringstream errorMsg{};

  do {
    OpCode opcode = static_cast<OpCode>(*(this->ip));

    switch (opcode) {
      case OpCode::NoOp:
        break;

      case OpCode::LoadConst:
        this->push(this->readConst());
        break;

      case OpCode::INeg: {
        gc::Val val = this->pop();
        CHECK_TYPE(
            val, gc::ValType::Int,
            "Expected an integer for this opcode: " << HEX_FORMAT(static_cast<uint8_t>(opcode)));
        this->push(this->gc->newInt(-(val.integer)));
        break;
      }

      case OpCode::FNeg: {
        gc::Val val = this->pop();
        CHECK_TYPE(
            val, gc::ValType::Float,
            "Expected a float for this opcode: " << HEX_FORMAT(static_cast<uint8_t>(opcode)));
        this->push(this->gc->newFloat(-(val.floatNum)));
        break;
      }

      case OpCode::BNeg: {
        gc::Val val = this->pop();
        CHECK_TYPE(
            val, gc::ValType::Bool,
            "Expected a boolean for this opcode: " << HEX_FORMAT(static_cast<uint8_t>(opcode)));
        this->push(this->gc->newBool(!(val.boolean)));
        break;
      }

      case OpCode::IAdd: {
        gc::Val val1 = this->pop();
        gc::Val val2 = this->pop();
        CHECK_TYPE(
            val1, gc::ValType::Int,
            "Expected an integer for this opcode: " << HEX_FORMAT(static_cast<uint8_t>(opcode)));
        CHECK_TYPE(
            val2, gc::ValType::Int,
            "Expected an integer for this opcode: " << HEX_FORMAT(static_cast<uint8_t>(opcode)));
        this->push(this->gc->newInt(val1.integer + val2.integer));
        break;
      }

      case OpCode::FAdd: {
        gc::Val val1 = this->pop();
        gc::Val val2 = this->pop();
        CHECK_TYPE(
            val1, gc::ValType::Float,
            "Expected a float for this opcode: " << HEX_FORMAT(static_cast<uint8_t>(opcode)));
        CHECK_TYPE(
            val2, gc::ValType::Float,
            "Expected a float for this opcode: " << HEX_FORMAT(static_cast<uint8_t>(opcode)));
        this->push(this->gc->newFloat(val1.floatNum + val2.floatNum));
        break;
      }

      case OpCode::SAdd: {
        gc::Val val1 = this->pop();
        gc::Val val2 = this->pop();
        CHECK_OBJ_TYPE(
            val1, gc::ObjType::Str,
            "Expected a string for this opcode: " << HEX_FORMAT(static_cast<uint8_t>(opcode)));
        CHECK_OBJ_TYPE(
            val2, gc::ObjType::Str,
            "Expected a string for this opcode: " << HEX_FORMAT(static_cast<uint8_t>(opcode)));
        std::string* str = val1.obj->str;
        val1.obj->str->append(*val2.obj->str);
        this->push(this->gc->newStr(str, stack));
        break;
      }

      case OpCode::ISub: {
        gc::Val val1 = this->pop();
        gc::Val val2 = this->pop();
        CHECK_TYPE(
            val1, gc::ValType::Int,
            "Expected an integer for this opcode: " << HEX_FORMAT(static_cast<uint8_t>(opcode)));
        CHECK_TYPE(
            val2, gc::ValType::Int,
            "Expected an integer for this opcode: " << HEX_FORMAT(static_cast<uint8_t>(opcode)));
        this->push(this->gc->newInt(val1.integer - val2.integer));
        break;
      }

      case OpCode::FSub: {
        gc::Val val1 = this->pop();
        gc::Val val2 = this->pop();
        CHECK_TYPE(
            val1, gc::ValType::Float,
            "Expected a float for this opcode: " << HEX_FORMAT(static_cast<uint8_t>(opcode)));
        CHECK_TYPE(
            val2, gc::ValType::Float,
            "Expected a float for this opcode: " << HEX_FORMAT(static_cast<uint8_t>(opcode)));
        this->push(this->gc->newFloat(val1.floatNum - val2.floatNum));
        break;
      }

      case OpCode::IMul: {
        gc::Val val1 = this->pop();
        gc::Val val2 = this->pop();
        CHECK_TYPE(
            val1, gc::ValType::Int,
            "Expected a float for this opcode: " << HEX_FORMAT(static_cast<uint8_t>(opcode)));
        CHECK_TYPE(
            val2, gc::ValType::Int,
            "Expected a float for this opcode: " << HEX_FORMAT(static_cast<uint8_t>(opcode)));
        this->push(this->gc->newInt(val1.integer * val2.integer));
        break;
      }

      case OpCode::FMul: {
        gc::Val val1 = this->pop();
        gc::Val val2 = this->pop();
        CHECK_TYPE(
            val1, gc::ValType::Float,
            "Expected a float for this opcode: " << HEX_FORMAT(static_cast<uint8_t>(opcode)));
        CHECK_TYPE(
            val2, gc::ValType::Float,
            "Expected a float for this opcode: " << HEX_FORMAT(static_cast<uint8_t>(opcode)));
        this->push(this->gc->newFloat(val1.floatNum * val2.floatNum));
        break;
      }

      case OpCode::FDiv: {
        gc::Val val1 = this->pop();
        gc::Val val2 = this->pop();
        CHECK_TYPE(
            val1, gc::ValType::Float,
            "Expected a float for this opcode: " << HEX_FORMAT(static_cast<uint8_t>(opcode)));
        CHECK_TYPE(
            val2, gc::ValType::Float,
            "Expected a float for this opcode: " << HEX_FORMAT(static_cast<uint8_t>(opcode)));
        this->push(this->gc->newFloat(val1.floatNum / val2.floatNum));
        break;
      }

      case OpCode::IMod: {
        gc::Val val1 = this->pop();
        gc::Val val2 = this->pop();
        CHECK_TYPE(
            val1, gc::ValType::Int,
            "Expected an integer for this opcode: " << HEX_FORMAT(static_cast<uint8_t>(opcode)));
        CHECK_TYPE(
            val2, gc::ValType::Int,
            "Expected an integer for this opcode: " << HEX_FORMAT(static_cast<uint8_t>(opcode)));
        this->push(this->gc->newInt(val1.integer % val2.integer));
        break;
      }

      case OpCode::FMod: {
        gc::Val val1 = this->pop();
        gc::Val val2 = this->pop();
        CHECK_TYPE(
            val1, gc::ValType::Float,
            "Expected a float for this opcode: " << HEX_FORMAT(static_cast<uint8_t>(opcode)));
        CHECK_TYPE(
            val2, gc::ValType::Float,
            "Expected a float for this opcode: " << HEX_FORMAT(static_cast<uint8_t>(opcode)));
        this->push(this->gc->newFloat(fmod(val1.floatNum, val2.floatNum)));
        break;
      }

      case OpCode::Pop:
        this->pop();
        break;

      case OpCode::PopN:
        this->pop(this->readByte());
        break;

      case OpCode::InitList: {
        int length = this->readByte();
        std::vector<gc::Val>* list = new std::vector<gc::Val>();
        list->reserve(length);
        for (int i = 0; i < length; i++) list->push_back(this->readConst());
        this->push(this->gc->newList(list, this->stack));
        break;
      }

      case OpCode::Jump:
        this->jump(this->readByte());
        break;

      case OpCode::LJump:
        this->jump(this->readTwoBytes());
        break;

      case OpCode::JumpIfFalse: {
        gc::Val val1 = this->pop();
        CHECK_TYPE(
            val1, gc::ValType::Bool,
            "Expected a boolean for this opcode: " << HEX_FORMAT(static_cast<uint8_t>(opcode)));
        if (!val1.boolean) this->jump(this->readTwoBytes());
        break;
      }

      case OpCode::BEq: {
        gc::Val val1 = this->pop();
        gc::Val val2 = this->pop();
        CHECK_TYPE(
            val1, gc::ValType::Bool,
            "Expected a boolean for this opcode: " << HEX_FORMAT(static_cast<uint8_t>(opcode)));
        CHECK_TYPE(
            val2, gc::ValType::Bool,
            "Expected a boolean for this opcode: " << HEX_FORMAT(static_cast<uint8_t>(opcode)));
        this->push(this->gc->newBool(val1.boolean == val2.boolean));
        break;
      }

      case OpCode::IEq: {
        gc::Val val1 = this->pop();
        gc::Val val2 = this->pop();
        CHECK_TYPE(
            val1, gc::ValType::Int,
            "Expected an integer for this opcode: " << HEX_FORMAT(static_cast<uint8_t>(opcode)));
        CHECK_TYPE(
            val2, gc::ValType::Int,
            "Expected an integer for this opcode: " << HEX_FORMAT(static_cast<uint8_t>(opcode)));
        this->push(this->gc->newBool(val1.integer == val2.integer));
        break;
      }

      case OpCode::FEq: {
        gc::Val val1 = this->pop();
        gc::Val val2 = this->pop();
        CHECK_TYPE(
            val1, gc::ValType::Float,
            "Expected a float for this opcode: " << HEX_FORMAT(static_cast<uint8_t>(opcode)));
        CHECK_TYPE(
            val2, gc::ValType::Float,
            "Expected a float for this opcode: " << HEX_FORMAT(static_cast<uint8_t>(opcode)));
        this->push(this->gc->newBool(val1.floatNum == val2.floatNum));
        break;
      }

      case OpCode::StrEq: {
        gc::Val val1 = this->pop();
        gc::Val val2 = this->pop();
        CHECK_OBJ_TYPE(
            val1, gc::ObjType::Str,
            "Expected a string for this opcode: " << HEX_FORMAT(static_cast<uint8_t>(opcode)));
        CHECK_OBJ_TYPE(
            val2, gc::ObjType::Str,
            "Expected a string for this opcode: " << HEX_FORMAT(static_cast<uint8_t>(opcode)));
        this->push(this->gc->newBool(*(val1.obj->str) == *(val2.obj->str)));
        break;
      }

      case OpCode::IGT: {
        gc::Val val1 = this->pop();
        gc::Val val2 = this->pop();
        CHECK_TYPE(
            val1, gc::ValType::Int,
            "Expected an integer for this opcode: " << HEX_FORMAT(static_cast<uint8_t>(opcode)));
        CHECK_TYPE(
            val2, gc::ValType::Int,
            "Expected an integer for this opcode: " << HEX_FORMAT(static_cast<uint8_t>(opcode)));
        this->push(this->gc->newBool(val1.integer > val2.integer));
        break;
      }

      case OpCode::FGT: {
        gc::Val val1 = this->pop();
        gc::Val val2 = this->pop();
        CHECK_TYPE(
            val1, gc::ValType::Float,
            "Expected a float for this opcode: " << HEX_FORMAT(static_cast<uint8_t>(opcode)));
        CHECK_TYPE(
            val2, gc::ValType::Float,
            "Expected a float for this opcode: " << HEX_FORMAT(static_cast<uint8_t>(opcode)));
        this->push(this->gc->newBool(val1.floatNum > val2.floatNum));
        break;
      }

      case OpCode::StrGT: {
        gc::Val val1 = this->pop();
        gc::Val val2 = this->pop();
        CHECK_OBJ_TYPE(
            val1, gc::ObjType::Str,
            "Expected a string for this opcode: " << HEX_FORMAT(static_cast<uint8_t>(opcode)));
        CHECK_OBJ_TYPE(
            val2, gc::ObjType::Str,
            "Expected a string for this opcode: " << HEX_FORMAT(static_cast<uint8_t>(opcode)));
        this->push(this->gc->newBool((val1.obj->str->compare(*val2.obj->str) > 0)));
        break;
      }

      case OpCode::ILT: {
        gc::Val val1 = this->pop();
        gc::Val val2 = this->pop();
        CHECK_TYPE(
            val1, gc::ValType::Int,
            "Expected an integer for this opcode: " << HEX_FORMAT(static_cast<uint8_t>(opcode)));
        CHECK_TYPE(
            val2, gc::ValType::Int,
            "Expected an integer for this opcode: " << HEX_FORMAT(static_cast<uint8_t>(opcode)));
        this->push(this->gc->newBool(val1.integer < val2.integer));
        break;
      }

      case OpCode::FLT: {
        gc::Val val1 = this->pop();
        gc::Val val2 = this->pop();
        CHECK_TYPE(
            val1, gc::ValType::Float,
            "Expected a float for this opcode: " << HEX_FORMAT(static_cast<uint8_t>(opcode)));
        CHECK_TYPE(
            val2, gc::ValType::Float,
            "Expected a float for this opcode: " << HEX_FORMAT(static_cast<uint8_t>(opcode)));
        this->push(this->gc->newBool(val1.floatNum < val2.floatNum));
        break;
      }

      case OpCode::StrLT: {
        gc::Val val1 = this->pop();
        gc::Val val2 = this->pop();
        CHECK_OBJ_TYPE(
            val1, gc::ObjType::Str,
            "Expected a string for this opcode: " << HEX_FORMAT(static_cast<uint8_t>(opcode)));
        CHECK_OBJ_TYPE(
            val2, gc::ObjType::Str,
            "Expected a string for this opcode: " << HEX_FORMAT(static_cast<uint8_t>(opcode)));
        this->push(this->gc->newBool((val1.obj->str->compare(*val2.obj->str) < 0)));
        break;
      }

      case OpCode::IGTEq: {
        gc::Val val1 = this->pop();
        gc::Val val2 = this->pop();
        CHECK_TYPE(
            val1, gc::ValType::Int,
            "Expected an integer for this opcode: " << HEX_FORMAT(static_cast<uint8_t>(opcode)));
        CHECK_TYPE(
            val2, gc::ValType::Int,
            "Expected an integer for this opcode: " << HEX_FORMAT(static_cast<uint8_t>(opcode)));
        this->push(this->gc->newBool(val1.integer > val2.integer));
        break;
      }

      case OpCode::FGTEq: {
        gc::Val val1 = this->pop();
        gc::Val val2 = this->pop();
        CHECK_TYPE(
            val1, gc::ValType::Float,
            "Expected a float for this opcode: " << HEX_FORMAT(static_cast<uint8_t>(opcode)));
        CHECK_TYPE(
            val2, gc::ValType::Float,
            "Expected a float for this opcode: " << HEX_FORMAT(static_cast<uint8_t>(opcode)));
        this->push(this->gc->newBool(val1.floatNum > val2.floatNum));
        break;
      }

      case OpCode::StrGTEq: {
        gc::Val val1 = this->pop();
        gc::Val val2 = this->pop();
        CHECK_OBJ_TYPE(
            val1, gc::ObjType::Str,
            "Expected a string for this opcode: " << HEX_FORMAT(static_cast<uint8_t>(opcode)));
        CHECK_OBJ_TYPE(
            val2, gc::ObjType::Str,
            "Expected a string for this opcode: " << HEX_FORMAT(static_cast<uint8_t>(opcode)));
        this->push(this->gc->newBool((val1.obj->str->compare(*val2.obj->str) >= 0)));
        break;
      }

      case OpCode::ILTEq: {
        gc::Val val1 = this->pop();
        gc::Val val2 = this->pop();
        CHECK_TYPE(
            val1, gc::ValType::Int,
            "Expected an integer for this opcode: " << HEX_FORMAT(static_cast<uint8_t>(opcode)));
        CHECK_TYPE(
            val2, gc::ValType::Int,
            "Expected an integer for this opcode: " << HEX_FORMAT(static_cast<uint8_t>(opcode)));
        this->push(this->gc->newBool(val1.integer < val2.integer));
        break;
      }

      case OpCode::FLTEq: {
        gc::Val val1 = this->pop();
        gc::Val val2 = this->pop();
        CHECK_TYPE(
            val1, gc::ValType::Float,
            "Expected a float for this opcode: " << HEX_FORMAT(static_cast<uint8_t>(opcode)));
        CHECK_TYPE(
            val2, gc::ValType::Float,
            "Expected a float for this opcode: " << HEX_FORMAT(static_cast<uint8_t>(opcode)));
        this->push(this->gc->newBool(val1.floatNum < val2.floatNum));
        break;
      }

      case OpCode::StrLTEq: {
        gc::Val val1 = this->pop();
        gc::Val val2 = this->pop();
        CHECK_OBJ_TYPE(
            val1, gc::ObjType::Str,
            "Expected a string for this opcode: " << HEX_FORMAT(static_cast<uint8_t>(opcode)));
        CHECK_OBJ_TYPE(
            val2, gc::ObjType::Str,
            "Expected a string for this opcode: " << HEX_FORMAT(static_cast<uint8_t>(opcode)));
        this->push(this->gc->newBool((val1.obj->str->compare(*val2.obj->str) <= 0)));
        break;
      }
      case OpCode::And: {
        gc::Val val1 = this->pop();
        gc::Val val2 = this->pop();
        CHECK_TYPE(
            val1, gc::ValType::Bool,
            "Expected a boolean for this opcode: " << HEX_FORMAT(static_cast<uint8_t>(opcode)));
        CHECK_TYPE(
            val2, gc::ValType::Bool,
            "Expected a boolean for this opcode: " << HEX_FORMAT(static_cast<uint8_t>(opcode)));
        this->push(this->gc->newBool(val1.boolean && val2.boolean));
        break;
      }

      case OpCode::Or: {
        gc::Val val1 = this->pop();
        gc::Val val2 = this->pop();
        CHECK_TYPE(
            val1, gc::ValType::Bool,
            "Expected a boolean for this opcode: " << HEX_FORMAT(static_cast<uint8_t>(opcode)));
        CHECK_TYPE(
            val2, gc::ValType::Bool,
            "Expected a boolean for this opcode: " << HEX_FORMAT(static_cast<uint8_t>(opcode)));
        this->push(this->gc->newBool(this->pop().boolean || this->pop().boolean));
        break;
      }

      case OpCode::ILoad0:
        this->push(this->gc->newInt(0));
        break;

      case OpCode::ILoad1:
        this->push(this->gc->newInt(1));
        break;

      case OpCode::ILoad2:
        this->push(this->gc->newInt(2));
        break;

      case OpCode::ILoad3:
        this->push(this->gc->newInt(3));
        break;

      case OpCode::FLoad0:
        this->push(this->gc->newFloat(0.0));
        break;

      case OpCode::FLoad1:
        this->push(this->gc->newFloat(1.0));
        break;

      case OpCode::FLoad2:
        this->push(this->gc->newFloat(2.0));
        break;

      case OpCode::FLoad3:
        this->push(this->gc->newFloat(3.0));
        break;

      case OpCode::LoadTrue:
        this->push(this->gc->newBool(true));
        break;

      case OpCode::LoadFalse:
        this->push(this->gc->newBool(false));
        break;

      case OpCode::LoadVoid:
        this->push(this->gc->newVoid());
        break;

      case OpCode::SetGlobal:
        this->setGlobal(this->readConst(), this->readConst());
        break;

      case OpCode::GetGlobal: {
        gc::Val name = this->readConst();
        this->getGlobal(name);
        break;
      }

      case OpCode::SetLocal:
        this->stack.at(this->readByte()) = this->peek(0);
        break;

      case OpCode::GetLocal:
        this->push(this->stack.at(this->readByte()));
        break;

      case OpCode::Halt:
        halt = true;
        break;

      default:
        errorMsg << "Invalid opcode: " << HEX_FORMAT(static_cast<uint8_t>(opcode));
        halt = true;
        break;
    }
  } while (this->nextIp() && (!halt));

  return errorMsg.str();
}

bool VM::nextIp() {
  if (this->ip == this->instructions->end()) return false;
  this->ip++;
  return true;
}

std::uint8_t VM::readByte() {
  this->nextIp();
  return *(this->ip);
}

std::uint16_t VM::readTwoBytes() {
  return util::fromLittleEndian({this->readByte(), this->readByte()});
}

void VM::push(gc::Val val) { this->stack.push_back(val); }

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

gc::Val VM::peek(uint8_t peek) { return this->stack.at(this->stack.size() - 1 - peek); }

void VM::jump(int32_t jump) { this->ip = this->ip + jump; }

gc::Val VM::readConst() { return this->consts->at(this->readTwoBytes()); }

void VM::setGlobal(gc::Val name, gc::Val value) {
  this->globals.insert_or_assign(*name.obj->str, value);
}

gc::Val VM::getGlobal(gc::Val& name) { return this->globals.at(*name.obj->str); }
