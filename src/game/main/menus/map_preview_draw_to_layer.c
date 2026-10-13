#include "types.h"
#include "game/map_preview.h"

extern u8 data_02149280[];

void Palette_Load(const void* src, int offset, u32 size);
void Gfx_RequestLayerCopy(int layer);
void func_02020848(void* dst);
void func_02020854(void* dst);
u16* Bg_GetMapEntryPtr(int layer, int x, int y);
void MapPreview_FillTileMap(u16* screen, int firstTile);

/* Draws the map preview into the character data at charBase (tile firstTile
 * on) and into layer's screen at (x, y), with palette row palette. */
void MapPreview_DrawToLayer(u8* charBase, int firstTile, int layer, int x,
                            int y, int palette) {
  u8* tiles;

  data_022a6b3c = palette;
  Palette_Load(data_02149280, (u16)(palette << 5), 32);
  tiles = charBase + firstTile * 32;
  func_02020854(tiles);
  func_02020848(tiles);
  MapPreview_FillTileMap(Bg_GetMapEntryPtr(layer, x, y),
                         firstTile | (palette << 12));
  Gfx_RequestLayerCopy(layer);
}
