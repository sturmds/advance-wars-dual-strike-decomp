#include "types.h"

extern u8 data_02149280[];
/* also declared in src/game/main/menus/map_preview_draw_to_layer.c and 1 more */
u32 data_022a6b3c;
/* picture of a tile outside the map; size: up to the next symbol */
u16 data_022a6b40[0x20];
/* offset of each row of the block; size: up to the next symbol */
u16 gMapPreviewRowOffsets[0x28];
u16 gMapPreviewColumnOffsets[0x30]; /* size: up to the next symbol */

int MainLoop_GetFrameCount(void);
void Palette_LoadWithCopy(const void* src, int offset, u32 size);

/* Frame callback: every other frame steps colour 14 of the preview's palette
 * row (data_022a6b3c) through the 16 colours at data_02149280 + 32. */
void MapPreview_CycleBorderColour(void) {
  int frame = MainLoop_GetFrameCount();
  int offset = data_022a6b3c * 32 + 28;

  /* The +32 is added at run time to match. */
  Palette_LoadWithCopy((u8*)((u32)data_02149280 + 32) + ((frame & 0x3c) >> 1),
                       (u16)offset, 2);
}
