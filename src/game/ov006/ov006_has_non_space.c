#include "types.h"

/* 1 if the string has a character other than a space, else 0. */
int Ov006_HasNonSpace(u8* text) {
  u8 c = *text;

  while (c != 0) {
    if (c == ' ') {
      text++;
    } else {
      return 1;
    }
    c = *text;
  }
  return 0;
}
