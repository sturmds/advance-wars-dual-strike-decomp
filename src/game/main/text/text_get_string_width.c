#include "types.h"

int Text_GetCharWidth(int ch);

/* Width in pixels of a string: the sum of each character's width. */
int Text_GetStringWidth(const u8* str) {
  int width = 0;

  while (*str != 0) {
    width += Text_GetCharWidth(*str);
    str++;
  }
  return width;
}
