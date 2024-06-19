#ifndef UTIL_H
#define UTIL_H

#include <cstdint>
#include <tuple>

namespace util {
  uint16_t fromLittleEndian(std::tuple<uint8_t, uint8_t> encoded);
  std::tuple<uint8_t, uint8_t> toLittleEndian(uint16_t number);
}

#endif /* UTIL_H */
