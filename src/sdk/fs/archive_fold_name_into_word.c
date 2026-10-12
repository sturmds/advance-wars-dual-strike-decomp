#include "sdk/fs.h"

/* Packs up to 3 characters of an archive name into one word, lower-cased. */
u32 Archive_FoldNameIntoWord(const char* name, int length) {
  u32 packed = 0;
  int i;

  if (length <= 3) {
    for (i = 0; i < length; i++) {
      u32 c = (u8)name[i];
      u32 fromA;

      if (c == 0) {
        break;
      }
      /* fromA is a separate variable to match */
      fromA = c - 'A';
      c = (fromA <= 'Z' - 'A') ? fromA + 'a' : fromA + 'A';
      packed |= c << (i * 8);
    }
  }
  return packed;
}
