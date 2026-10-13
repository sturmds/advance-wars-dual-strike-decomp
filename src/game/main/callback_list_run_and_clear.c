#include "types.h"
#include "game/callback.h"

void CallbackList_Clear(void);

/* Calls every callback added by CallbackList_Add in order, then empties the list. */
void CallbackList_RunAndClear(void) {
  u8 i;

  for (i = 0; i < gCallbackCount; i++) {
    gCallbackList[i]();
  }
  CallbackList_Clear();
}
