#include "types.h"

int MainLoop_GetFrameCount(void);
void Mem_CopyAuto(const void* src, void* dst, u32 size);

extern u8 gDialogueCursorTiles[];
extern u8* data_02297790;

/* Cycles the blinking dialogue cursor over 46 frames through tile sets 0, 1, 2,
 * 1 and copies the current one into the text layer's tile buffer. */
void DialogueCursor_Animate(void) {
  u32 frame = MainLoop_GetFrameCount() % 46;
  int step;

  if (frame < 12) {
    step = 0;
  } else if (frame < 18) {
    step = 1;
  } else if (frame < 40) {
    step = 2;
  } else {
    step = 1;
  }
  Mem_CopyAuto(&gDialogueCursorTiles[step * 32], data_02297790 + 0x3600, 32);
  Mem_CopyAuto(&gDialogueCursorTiles[(step * 2 + 3) * 32],
               data_02297790 + 0x3620, 64);
}
