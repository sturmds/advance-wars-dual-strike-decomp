#include "types.h"
#include "game/event_script.h"

void EventScript_StartQueuedIfRequested(EventScriptSlot* slot);

/* Frees script slot number slot. */
void EventScript_Stop(s16 slot) {
  EventScript_StartQueuedIfRequested(&data_022977f0[slot]);
}
