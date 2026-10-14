#include "types.h"
#include "game/oam_cells.h"

void Sprite_Queue(int list, int x, int y, void* data, int attr, int tile);
int Ov003_GetGlyphWidth(int ch);

extern u8 data_ov003_02364b60[];

/* Same as Ov003_DrawLetter, with the same letter list in
 * data_ov003_02364b60 and the tiles starting at 0x58e. */
int Ov003_GetGlyphWidth(int ch);

extern u8 data_ov003_02364b60[];

/* Queues the glyph for character ch and returns its advance width, or 0 for
 * a character not in the list. The list data_ov003_02364b60 holds the
 * letters of this font ("COMBATSR!FINHDEL"), 16 tiles per position. x and y are OAM position words; bit 9 of y
 * marks a double-size affine sprite, which is moved up and left by 16. */
int Ov003_DrawLetterAlt(int ch, u16 x, u16 y) {
  int index = 0;

  if (y & 0x200) {
    x = ((x & 0x1ff) - 16 & 0x1ff) | (x & 0xfe00);
    y = ((y & 0xff) - 16 & 0xff) | (y & 0xff00);
  }
  for (; data_ov003_02364b60[index] != 0; index++) {
    if (data_ov003_02364b60[index] == ch) {
      Sprite_Queue(1, x, y, &data_027c0048, 0x2000, 0x58e + index * 16);
      return Ov003_GetGlyphWidth(ch);
    }
  }
  return 0;
}
