/* Built without optimisation in the original; see event_triggers_run.c. */
#pragma optimization_level 0

#include "types.h"
#include "game/event_script.h"

/* Steps forward to the entry that closes the current block (an
 * EVENT_TRIGGER_RUN kind, 23-25, or EVENT_TRIGGER_END) and returns it. */
const EventTrigger* EventTriggers_SkipBlock(const EventTrigger* trigger) {
  while (trigger->kind != EVENT_TRIGGER_RUN &&
         trigger->kind != EVENT_TRIGGER_RUN + 1 &&
         trigger->kind != EVENT_TRIGGER_RUN + 2) {
    if (trigger->kind == EVENT_TRIGGER_END) {
      return trigger;
    }
    trigger++;
  }
  return trigger;
}
