#ifndef UTIL_H
#define UTIL_H

#include <cstdint>
#include <tuple>

namespace util {
uint16_t fromLittleEndian(std::tuple<uint8_t, uint8_t> encoded);
int64_t fromLittleEndian(uint8_t *encoded);
double fromLittleEndianToFloat(uint8_t *encoded);
std::tuple<uint8_t, uint8_t> toLittleEndian(uint16_t number);
} // namespace util

#endif /* UTIL_H */
