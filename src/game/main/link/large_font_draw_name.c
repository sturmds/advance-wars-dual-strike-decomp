#include "types.h"
#include "game/Unk_02088d04.h"
#include "game/link_player_name.h"

void Mem_Zero32(u32 value, void* dst, u32 size);
void Mem_Copy32(const void* src, void* dst, u32 size);
int LargeFont_FindGlyph(u16 c);

/* Glyphs of the 16x16 font that LargeFont_Load unpacks into LCDC VRAM bank A,
 * 4 tiles (LARGE_FONT_GLYPH_SIZE bytes) each. */
#define LARGE_FONT_GLYPHS ((const u8*)0x6800000)
#define LARGE_FONT_GLYPH_SIZE 0x80

static inline u8* Layer0Tile(Unk_027c0280* gfx, int tile) {
  return gfx->bgVram + (gfx->bgCnt[gfx->layer0Bg].charBase << 14) + (tile << 5);
}

/* Draws a 10-character name in the 16x16 font at map cell (x, y) of layer 0.
 * Each character takes 4 BG tiles from firstTile on and a 2x2 block of map cells. */
void LargeFont_DrawName(const u16* name, int x, int y, int firstTile) {
  int i;
  int cell;
  int tile;
  Unk_027c0280* gfx;

  Mem_Zero32(0, Layer0Tile(data_027c0280, firstTile),
             LINK_PLAYER_NAME_LENGTH * LARGE_FONT_GLYPH_SIZE);
  cell = x + y * 32;
  for (i = 0; i < LINK_PLAYER_NAME_LENGTH; i++, name++) {
    gfx = data_027c0280;
    tile = firstTile + i * 4;
    Mem_Copy32(
        LARGE_FONT_GLYPHS + LargeFont_FindGlyph(*name) * LARGE_FONT_GLYPH_SIZE,
        Layer0Tile(gfx, tile), LARGE_FONT_GLYPH_SIZE);
    data_027c0280->layer0Map[cell + i * 2] = tile;
    data_027c0280->layer0Map[cell + i * 2 + 1] = tile + 1;
    data_027c0280->layer0Map[cell + i * 2 + 32] = tile + 2;
    data_027c0280->layer0Map[cell + i * 2 + 33] = tile + 3;
  }
}
