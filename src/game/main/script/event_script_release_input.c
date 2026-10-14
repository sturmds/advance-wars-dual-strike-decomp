#include "types.h"

extern s16 gEventScriptOwnsInput;

/* Gives keys and touch back to the player (see EventScript_TakeInput). */
void* EventScript_ReleaseInput(void) {
  gEventScriptOwnsInput = 0;
  return &gEventScriptOwnsInput;
}
