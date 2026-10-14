#include "types.h"
#include "game/Unk_02088d04.h"

int MainLoop_GetFrameCount(void);
void Mem_CopyAuto(const void* src, void* dst, u32 size);

extern u8 gDialogueCursorTiles[];
extern u8* data_02297790;

/* gDialogueCursorTiles holds the three sets as one tile each, then as two
 * tiles each; the cursor shows at text layer tile CURSOR_TILE and the two
 * tiles after it. */
#define CURSOR_TILE 0x1b0

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
  Mem_CopyAuto(&gDialogueCursorTiles[step * TILE_4BPP_SIZE],
               data_02297790 + CURSOR_TILE * TILE_4BPP_SIZE, TILE_4BPP_SIZE);
  Mem_CopyAuto(&gDialogueCursorTiles[(step * 2 + 3) * TILE_4BPP_SIZE],
               data_02297790 + (CURSOR_TILE + 1) * TILE_4BPP_SIZE,
               2 * TILE_4BPP_SIZE);
}
