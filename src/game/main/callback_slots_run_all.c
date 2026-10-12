#include "types.h"

/* volatile: needed to match, the original reads the slot again before the call. */
extern void (*volatile gCallbackSlots[16])(void);

/* Calls every callback stored in the 16 slots, in order (called from the
 * vblank handler). */
void CallbackSlots_RunAll(void) {
  u16 i;

  for (i = 0; i < 16; i++) {
    if (gCallbackSlots[i] != NULL) {
      gCallbackSlots[i]();
    }
  }
}
