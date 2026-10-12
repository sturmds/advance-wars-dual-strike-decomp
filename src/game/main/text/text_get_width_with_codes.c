#include "types.h"

extern u8 Ov000_gGlyphWidths[]; /* glyph widths in pixels, by character code */

/* Width in pixels of a string, with 1 pixel between glyphs. Codes 9 and 10
 * take 16 pixels and a parameter byte after them. */
int Text_GetWidthWithCodes(const u8* text) {
  int width;
  BOOL started;
  u8 c;

  width = 0;
  started = FALSE;
  c = *text;
  while (c != 0) {
    switch (c) {
      case 9:
      case 10:
        width += 16;
        text += 2;
        break;
      default:
        if (started) {
          width++;
        }
        width += Ov000_gGlyphWidths[c];
        started = TRUE;
        text++;
        break;
    }
    c = *text;
  }
  return width;
}
