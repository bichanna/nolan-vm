#include <stddef.h>

#include "./value.h"

size_t utf8_chars_length(const char* chars);
size_t objstr_length(const ObjStr* str);
bool validate_utf8(const char* chars);
