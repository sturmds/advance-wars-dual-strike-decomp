#include "types.h"
#include "game/script_slot.h"

void ScreenWipe_SetColumn(int column, int phase, int mirror);
void Gfx_RequestLayerCopy(int layer);

/* Script callback of a screen wipe made of 2x2-tile blocks, mirrored. Each
 * frame, column c before the frame number gets tile phase frame - c, so the
 * phases rise as the frame grows. The wipe ends at frame 22. */
void ScreenWipe_PhaseUpStep(WipeSlot* slot) {
  u8 column = 0;
  s16 step = slot->frame;

  for (; column < slot->frame; column++) {
    if (column >= 16) {
      break;
    }
    ScreenWipe_SetColumn(column, step--, 1);
  }
  Gfx_RequestLayerCopy(0);
  if (slot->frame == 22) {
    slot->callback = NULL;
  } else {
    slot->frame++;
  }
}
