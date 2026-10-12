#include "types.h"
#include "game/text_print.h"

extern u8 Ov000_gGlyphWidths[]; /* glyph widths in pixels, by character code */

int Text_GetPrintWidth(const void* text) {
  const u8* p = text;
  u16 width = 0;

  while (*p != 0) {
    if (width != 0) {
      width++;
    }
    width += Ov000_gGlyphWidths[*p];
    p++;
  }
  return width;
}
