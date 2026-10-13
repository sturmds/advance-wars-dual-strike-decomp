#include "types.h"
#include "game/callback.h"

/* Removes a callback from the list of 16; nothing happens if it is not there. */
void FrameCallback_Remove(void (*callback)(void)) {
  u8 i;

  for (i = 0; i < CALLBACK_SLOTS; i++) {
    if (gCallbackSlots[i] == callback) {
      gCallbackSlots[i] = NULL;
      return;
    }
  }
}
