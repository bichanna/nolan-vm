#include "./le.h"

#include <string.h>

inline uint16_t le_to_ui32(const uint8_t encoded[2]) {
  uint16_t result;
  memcpy(&result, encoded, sizeof(uint16_t));
  return result;
}

inline uint32_t le_to_ui24(const uint8_t encoded[3]) {
  return (uint32_t)encoded[0] | ((uint32_t)encoded[1] << 8) | ((uint32_t)encoded[2] << 16);
}

inline int64_t le_to_i64(const uint8_t encoded[8]) {
  int64_t result;
  memcpy(&result, encoded, sizeof(int64_t));
  return result;
}

inline double le_to_f64(const uint8_t encoded[8]) {
  double result;
  memcpy(&result, encoded, sizeof(double));
  return result;
}
