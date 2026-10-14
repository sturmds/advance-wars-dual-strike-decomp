/* Built without optimisation in the original; see event_triggers_run.c. */
#pragma optimization_level 0

#include "types.h"
#include "game/event_script.h"

/* Steps forward to the entry that closes the current block (an
 * EVENT_TRIGGER_RUN kind in any of its three forms, or EVENT_TRIGGER_END)
 * and returns it. */
const EventTrigger* EventTriggers_SkipBlock(const EventTrigger* trigger) {
  while (trigger->kind != EVENT_TRIGGER_RUN &&
         trigger->kind != EVENT_TRIGGER_RUN + EVENT_KIND_SECOND_MAP &&
         trigger->kind != EVENT_TRIGGER_RUN + EVENT_KIND_BOTH_MAPS) {
    if (trigger->kind == EVENT_TRIGGER_END) {
      return trigger;
    }
    trigger++;
  }
  return trigger;
}
