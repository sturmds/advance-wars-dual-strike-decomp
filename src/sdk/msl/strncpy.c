#include "types.h"
#include "sdk/msl_wchar.h"

char* strncpy(char* dest, const char* src, u32 count) {
  char* out = dest;

  if (count == 0) {
    return dest;
  }

  do {
    if ((*out++ = *src++) == 0) {
      if (--count != 0) {
        do {
          *out++ = 0;
        } while (--count != 0);
      }
      return dest;
    }
  } while (--count != 0);

  return dest;
}
