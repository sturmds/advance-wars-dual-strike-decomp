#include "types.h"
#include "sdk/msl_string.h"

char* strchr(const char* str, int ch) {
  u32 cur = *(u8*)str;
  u32 c = ch & 0xFF;

  str++;
  while (cur != 0) {
    if (cur == c) {
      return (char*)(str - 1);
    }
    cur = *str++;
  }

  if (c != 0) {
    return NULL;
  }
  return (char*)(str - 1);
}
