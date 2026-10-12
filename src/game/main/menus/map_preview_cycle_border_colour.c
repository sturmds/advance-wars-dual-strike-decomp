#include "types.h"

extern u8 data_02149280[];
extern u32 data_022a6b3c;

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
