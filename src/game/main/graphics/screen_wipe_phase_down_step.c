#include "types.h"
#include "game/script_slot.h"

void ScreenWipe_SetColumn(int column, int phase, int mirror);
void Gfx_RequestLayerCopy(int layer);

/* Script callback of a screen wipe made of 2x2-tile blocks. Each frame,
 * column c up to the frame number gets tile phase 6 - frame + c, so the
 * phases fall as the frame grows (ScreenWipe_SetColumn clamps them to 0-6). The
 * wipe ends at frame 22. */
void ScreenWipe_PhaseDownStep(WipeSlot* slot) {
  u8 column = 0;
  s16 step = 6 - slot->frame;

  for (; column <= slot->frame; column++) {
    if (column >= 16) {
      break;
    }
    ScreenWipe_SetColumn(column, step++, 0);
  }
  Gfx_RequestLayerCopy(0);
  if (slot->frame == 22) {
    slot->callback = NULL;
  } else {
    slot->frame++;
  }
}
