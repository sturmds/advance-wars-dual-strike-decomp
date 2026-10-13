#include "types.h"
#include "game/callback.h"

/* Stores the callback in the first free slot of the 16-slot table
 * gCallbackSlots (which sits right before gCallbackList); ignored when full. */
void CallbackSlots_Add(void (*callback)(void)) {
  void (*volatile * slot)(void);

  for (slot = gCallbackSlots; slot < gCallbackList; slot++) {
    if (*slot == 0) {
      *slot = callback;
      return;
    }
  }
}
