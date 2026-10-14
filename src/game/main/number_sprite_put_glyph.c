#include "types.h"
#include "game/oam_cells.h"

void Oam_PutSprite(int x, int y, void* shape, int tileAndPalette);

/* Puts one glyph sprite: glyphs sit 8 tiles apart from firstTile on. */
void NumberSprite_PutGlyph(int x, int y, int glyph, int firstTile,
                           int palette) {
  Oam_PutSprite(x, y, &data_027c0000, firstTile + glyph * 8 | palette << 12);
}
