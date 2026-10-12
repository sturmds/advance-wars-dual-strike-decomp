#include "types.h"
#include "game/event_script.h"

/* Returns the first slot running script, or -1. A NULL script finds a free slot. */
s16 EventScript_FindSlot(const EventScriptCommand* script) {
  s16 i;

  for (i = 0; i < EVENT_SCRIPT_SLOTS; i++) {
    if (data_022977f0[i].start == script) {
      return i;
    }
  }
  return -1;
}
