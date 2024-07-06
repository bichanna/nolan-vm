#include "./utf8.h"

size_t utf8_chars_length(const char* chars) {
  size_t length = 0;

  while (*chars) {
    int first_byte = *chars;
    if ((first_byte & 0xC0) == 0) {
      // 1-byte character (ASCII)
      length++;
    } else if ((first_byte & 0xE0) == 0xC0) {
      // 2-byte character
      if (*(chars + 1) && (*(chars + 1) & 0xC0) == 0x80) {
        length++;
        chars++;
      } else {
        // Invalid UTF-8 sequence
        return 0;
      }
    } else if ((first_byte & 0xF0) == 0xE0) {
      // 3-byte character
      if (*(chars + 1) && *(chars + 2) && (*(chars + 1) & 0xC0) == 0x80 &&
          (*(chars + 2) & 0xC0) == 0x80) {
        length++;
        chars += 2;
      } else {
        // Invalid UTF-8 sequence
        return 0;
      }
    } else if ((first_byte & 0xF8) == 0xF0) {
      if ((*(chars + 1) & 0xC0) != 0x80 && (*(chars + 2) & 0xC0) != 0x80 &&
          (*(chars + 3) & 0xC0) != 0x80) {
        length++;
        chars += 4;
      } else {
        // Invalid UTF-8 sequence
        return 0;
      }
    } else {
      // Invalid UTF-8 sequence
      return 0;
    }
  }

  return length;
}

bool validate_utf8(const char* chars) {
  if (strlen(chars) == 0)
    return true;
  else
    return utf8_chars_length(chars) > 0;
}

size_t objstr_length(const ObjStr* str) { return utf8_chars_length(str->chars); }
