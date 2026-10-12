#include "types.h"

extern void (*gCallbackSlots[16])(void);
extern void (*gCallbackList[16])(void);

/* Stores the callback in the first free slot of the 16-slot table
 * gCallbackSlots (which sits right before gCallbackList); ignored when full. */
void CallbackSlots_Add(void (*callback)(void)) {
  void (**slot)(void);

  for (slot = gCallbackSlots; slot < gCallbackList; slot++) {
    if (*slot == 0) {
      *slot = callback;
      return;
    }
  }
}
