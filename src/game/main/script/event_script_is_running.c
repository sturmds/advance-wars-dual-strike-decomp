#include "types.h"
#include "game/event_script.h"

/* TRUE while any script slot is in use. */
BOOL EventScript_IsRunning(void) {
  s16 i;

  for (i = 0; i < EVENT_SCRIPT_SLOTS; i++) {
    if (data_022977f0[i].start != NULL) {
      return TRUE;
    }
  }
  return FALSE;
}
