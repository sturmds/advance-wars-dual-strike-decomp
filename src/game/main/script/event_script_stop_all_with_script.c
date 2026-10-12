#include "types.h"
#include "game/event_script.h"

void EventScript_Stop(s16 slot);

/* Frees every slot running script. Always returns -1; the one caller ignores it. */
s16 EventScript_StopAllWithScript(const EventScriptCommand* script) {
  s16 i;

  for (i = 0; i < EVENT_SCRIPT_SLOTS; i++) {
    if (data_022977f0[i].start == script) {
      EventScript_Stop(i);
    }
  }
  return -1;
}
