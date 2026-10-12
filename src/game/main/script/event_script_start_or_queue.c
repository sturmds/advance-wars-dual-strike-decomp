#include "types.h"
#include "game/event_script.h"

BOOL EventScript_IsRunning(void);

/* Starts script now when no script runs, otherwise puts it in the first free
 * queue entry (dropped when the queue is full). */
void EventScript_StartOrQueue(const EventScriptCommand* script) {
  int i;

  if (!EventScript_IsRunning()) {
    EventScript_Start(script);
    return;
  }
  for (i = 0; i < EVENT_SCRIPT_QUEUE_SIZE; i++) {
    if (gEventScriptQueue[i] == NULL) {
      gEventScriptQueue[i] = script;
      return;
    }
  }
}
