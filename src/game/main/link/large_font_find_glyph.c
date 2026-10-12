#include "types.h"

extern u16 gLargeFontCodes[];

/* Index of character code c in gLargeFontCodes (ended by 0), which is also the
 * glyph's index in the font; 0 when c is not listed. */
int LargeFont_FindGlyph(u16 c) {
  const u16* entry = gLargeFontCodes;
  u16 code;

  while ((code = *entry) != 0) {
    if (code == c) {
      return entry - gLargeFontCodes;
    }
    entry++;
  }
  return 0;
}
