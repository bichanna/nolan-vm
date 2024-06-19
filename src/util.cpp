#include "../include/util.hpp"

uint16_t util::fromLittleEndian(std::tuple<uint8_t, uint8_t> encoded) {
  auto [f, s] = encoded;
  return ((static_cast<uint8_t>(s) & 0xFF) << 8) | (static_cast<uint8_t>(f) & 0xFF);
}

std::tuple<uint8_t, uint8_t> util::toLittleEndian(uint16_t val) {
  return {
    static_cast<uint8_t>(val & 0xFF),
    static_cast<uint8_t>((val >> 8) & 0xFF)
  };
}
