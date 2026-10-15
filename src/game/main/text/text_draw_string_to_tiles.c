#include "types.h"
#include "game/text_print.h"
#include "game/math.h"

int gTextDrawTilePixel;
static int gTextDrawGlyphWidth;

int func_0202083c(int c, u8* tiles, int x, int colour);
BOOL Text_AdvanceDrawPos(int pixels);

/* Draws str into the 4-bit tiles at tiles, one pixel between characters.
 * A character that reaches the end of its tile moves tiles on by 64 bytes
 * (an 8x16 glyph column). Returns the number of tiles used: the width in
 * 8-pixel columns, times 2. */
int Text_DrawStringToTiles(const u8* str, u8* tiles, int colour) {
  gTextDrawGlyphWidth = 0;
  gTextDrawTilePixel = 0;
  gTextDrawWidth = 0;
  if (str[0] != 0) {
    do {
      if (gTextDrawWidth != 0) {
        if (Text_AdvanceDrawPos(1)) {
          tiles += 64;
        }
      }
      gTextDrawGlyphWidth =
          func_0202083c(*str++, tiles, gTextDrawTilePixel, colour);
      if (Text_AdvanceDrawPos(gTextDrawGlyphWidth)) {
        tiles += 64;
      }
    } while (*str != 0);
  }
  return HwDiv32(gTextDrawWidth + 7, 8) * 2;
}
