#include "../include/util.hpp"

#include <cstring>

uint16_t util::fromLittleEndian(std::tuple<uint8_t, uint8_t> encoded) {
  auto [f, s] = encoded;
  return ((static_cast<uint8_t>(s) & 0xFF) << 8) | (static_cast<uint8_t>(f) & 0xFF);
}

int64_t util::fromLittleEndian(uint8_t* encoded) {
  int64_t integer{};
  std::memcpy(&integer, encoded, sizeof(int64_t));
  return integer;
}

double util::fromLittleEndianToFloat(uint8_t* encoded) {
  uint64_t value{};
  std::memcpy(&value, encoded, sizeof(uint64_t));
  double floatNum;
  std::memcpy(&floatNum, &value, sizeof(double));
  return floatNum;
}

std::tuple<uint8_t, uint8_t> util::toLittleEndian(uint16_t val) {
  return {static_cast<uint8_t>(val & 0xFF), static_cast<uint8_t>((val >> 8) & 0xFF)};
}
