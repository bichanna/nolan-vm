#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>

inline uint16_t le_to_ui32(const uint8_t encoded[2]);
inline uint32_t le_to_ui24(const uint8_t encoded[3]);
inline int64_t le_to_i64(const uint8_t encoded[8]);
inline double le_to_f64(const uint8_t encoded[8]);

#endif
