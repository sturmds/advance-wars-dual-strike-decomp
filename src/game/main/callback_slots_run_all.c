#include "types.h"
#include "game/callback.h"

/* Calls every callback stored in the 16 slots, in order (called from the
 * vblank handler). */
void CallbackSlots_RunAll(void) {
  u16 i;

  for (i = 0; i < CALLBACK_SLOTS; i++) {
    if (gCallbackSlots[i] != NULL) {
      gCallbackSlots[i]();
    }
  }
}
