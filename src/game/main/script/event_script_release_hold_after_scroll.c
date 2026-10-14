#include "types.h"
#include "game/event_script.h"
#include "game/script_slot.h"

extern ScriptCommand gMapScrollScript[];
s16 ScriptSlot_Find(const ScriptCommand* script);

/* Hold callback set by EventScript_ScrollMapAndWait: lets the script go on once
 * no slot runs the scroll script gMapScrollScript any more. */
void EventScript_ReleaseHoldAfterScroll(EventScriptSlot* slot) {
  if (ScriptSlot_Find(gMapScrollScript) == -1) {
    slot->hold = 0;
  }
}
