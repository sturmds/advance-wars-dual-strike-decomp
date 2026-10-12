#include "types.h"

/* Copies a zero-terminated string (up to 255 characters). The loop copies
 * the next character too, which the next round overwrites; needed to match. */
void String_Copy(char* dest, const char* src) {
  u8 i = 0;

  while (src[i] != 0) {
    char c = src[i];
    const char* s = src + i;
    char* d = dest + i;
    dest[i] = c;
    d[1] = s[1];
    i++;
  }
  dest[i] = 0;
}
