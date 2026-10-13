#include "types.h"
#include "game/map_preview.h"

extern u8 data_02149280[];

void Palette_Load(const void* src, int offset, u32 size);
void MapPreview_DrawCurrentMap(void* dst);
void MapPreview_FillTileMap(u16* screen, int firstTile);

/* Like MapPreview_DrawToLayer, but fills the given screen map and draws the
 * preview with MapPreview_DrawCurrentMap. */
void MapPreview_DrawToScreen(u8* charBase, u16* screen, int firstTile,
                             int palette) {
  data_022a6b3c = palette;
  Palette_Load(data_02149280, (u16)(palette << 5), 32);
  MapPreview_DrawCurrentMap(charBase + firstTile * 32);
  MapPreview_FillTileMap(screen, firstTile | (palette << 12));
}
