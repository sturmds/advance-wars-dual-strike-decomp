#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

/* data_027c027c->unk_48[data_027c0284], written so that the compiler loads
 * the index first, as the original does. */
#define STATE                                                             \
  (*(Unk_02080440_48**)((u8*)(data_027c0284 * sizeof(Unk_02080440_48*)) + \
                            (u32)data_027c027c + (u32) &                  \
                        ((Unk_02080440*)0)->unk_48))

/* Clears the acting unit's capture progress when its move ends on a
 * different tile from where it started. */
void ActingUnit_ResetCaptureIfMoved(void) {
  Unk_02080440_48* state = STATE;

  if (state->actingUnitStartX == state->actingUnitDestX &&
      state->actingUnitStartY == state->actingUnitDestY) {
    return;
  }
  state->actingUnit->captureProgress = 0;
}
