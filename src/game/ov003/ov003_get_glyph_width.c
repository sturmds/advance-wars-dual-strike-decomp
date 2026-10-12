#include "types.h"

/* Width in pixels of a character in this overlay's text: '!' and 'I' are
 * narrow, every other character is wide. */
int Ov003_GetGlyphWidth(int ch) {
  if (ch == '!' || ch == 'I') {
    return 16;
  }
  return 32;
}
