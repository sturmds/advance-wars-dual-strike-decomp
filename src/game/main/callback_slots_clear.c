#include "types.h"

extern void (*gCallbackSlots[16])(void);

/* Empties the slot table filled by CallbackSlots_Add. */
void CallbackSlots_Clear(void) {
  u8 i;

  for (i = 0; i < 16; i++) {
    gCallbackSlots[i] = 0;
  }
}
