#include "types.h"
#include "game/callback.h"

/* Empties the list that CallbackList_Add fills. */
void CallbackList_Clear(void) {
  u8 i;

  gCallbackCount = 0;
  for (i = 0; i < CALLBACK_SLOTS; i++) {
    gCallbackList[i] = 0;
  }
}
