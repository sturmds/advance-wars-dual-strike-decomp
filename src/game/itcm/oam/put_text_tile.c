#include "types.h"
#include "game/oam_cells.h"

void Oam_PutSprite(int x, int y, void* shape, int tileAndPalette);

#define TEXT_PALETTE_BITS 0xf000

/* Puts one tile of the sprite font, drawn with palette 15. */
void Oam_PutTextTile(int x, int y, int tile) {
  Oam_PutSprite(x & 0x1ff, y & 0xff, &data_027c0028, tile | TEXT_PALETTE_BITS);
}
