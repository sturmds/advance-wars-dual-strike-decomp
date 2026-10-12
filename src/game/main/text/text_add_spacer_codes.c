#include "types.h"

extern u8 Ov000_gGlyphWidths[]; /* glyph widths in pixels, by character code */

/* Copies src to dst with spacer codes added, for the name entry screen. Before
 * a character, when x is part way into a tile, code 31 - (x & 7) is added
 * (codes 24 to 30). Before a character 1 to 6 pixels wide, code
 * 23 + (8 - width) / 2 is added. x starts each character as that character's
 * width (plus the padding), not as a running total. First sets src[i] to 0
 * at the first 0 byte from index 15 on. */
void Text_AddSpacerCodes(u8* src, u8* dst) {
  int c;
  int i;
  int x;
  int pad;

  x = 0;
  for (i = 0; src[i] != 0 || i < 15; i++) {
  }
  src[i] = 0;
  if (src[0] != 0) {
    do {
      if (x >= 1) {
        x++;
      }
      if (x & 7) {
        *dst++ = 31 - (x & 7);
      }
      c = *src++;
      x = Ov000_gGlyphWidths[c];
      switch (x) {
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
          pad = 8 - x;
          *dst++ = (pad >> 1) + 23;
          x += pad >> 1;
      }
      *dst++ = c;
    } while (*src != 0);
  }
  *dst = 0;
}
