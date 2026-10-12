#include "types.h"

/* Compares up to length bytes, ignoring ASCII case. Returns the difference
 * of the first pair that differs, or 0. */
int FSi_StrNICmp(const char* a, const char* b, int length) {
  int i;

  for (i = 0; i < length; i++) {
    u32 ca = (u8)a[i] - 'A';
    u32 cb = (u8)b[i] - 'A';

    if (ca <= 'Z' - 'A') {
      ca += 'a' - 'A';
    }
    if (cb <= 'Z' - 'A') {
      cb += 'a' - 'A';
    }
    if (ca != cb) {
      return ca - cb;
    }
  }
  return 0;
}
