#include "types.h"
#include "sdk/msl_wchar.h"

u32 wcstombs(char* dest, const wchar_t* src, u32 count) {
  u32 written = 0;
  u32 length;
  char buffer[4];
  char* temp;

  if (dest == NULL || src == NULL) {
    return 0;
  }

  temp = buffer;
  do {
    if (*src == 0) {
      dest[written] = 0;
      break;
    }
    length = wctomb(temp, *src++);
    if (written + length > count) {
      break;
    }
    strncpy(dest + written, temp, length);
    written += length;
  } while (written <= count);

  return written;
}
