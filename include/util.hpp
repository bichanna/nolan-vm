#ifndef UTIL_H
#define UTIL_H

#include <cstdint>
#include <tuple>

namespace util {
  uint16_t from_little_endian(std::tuple<uint8_t, uint8_t> encoded);
  std::tuple<uint8_t, uint8_t> to_little_endian(uint16_t number);
}

#endif /* UTIL_H */
