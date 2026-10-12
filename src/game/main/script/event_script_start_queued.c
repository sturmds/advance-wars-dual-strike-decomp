#include "types.h"
#include "game/event_script.h"

/* Starts the highest-numbered queued script and empties its queue entry. */
void EventScript_StartQueued(void) {
  int i;

  for (i = EVENT_SCRIPT_QUEUE_SIZE - 1; i >= 0; i--) {
    if (gEventScriptQueue[i] != NULL) {
      EventScript_Start(gEventScriptQueue[i]);
      gEventScriptQueue[i] = NULL;
      return;
    }
  }
}
