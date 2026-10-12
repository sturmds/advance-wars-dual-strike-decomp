#include "types.h"

int Ov003_GetGlyphWidth(int ch);

/* Width in pixels of a string in the overlay's letter font. */
int Ov003_GetTextWidth(const u8* str) {
  int width = 0;

  while (*str != 0) {
    width += Ov003_GetGlyphWidth(*str);
    str++;
  }
  return width;
}
