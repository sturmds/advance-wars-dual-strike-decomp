#include "types.h"
#include "game/callback.h"

/* Empties the slot table filled by CallbackSlots_Add. */
void CallbackSlots_Clear(void) {
  u8 i;

  for (i = 0; i < CALLBACK_SLOTS; i++) {
    gCallbackSlots[i] = 0;
  }
}
