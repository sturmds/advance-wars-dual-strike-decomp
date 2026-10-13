#include "types.h"
#include "game/callback.h"

/* Adds a callback to the list of up to 16; ignored when the list is full. */
void CallbackList_Add(void (*callback)(void)) {
  if (gCallbackCount == CALLBACK_SLOTS) {
    return;
  }
  gCallbackList[gCallbackCount++] = callback;
}
