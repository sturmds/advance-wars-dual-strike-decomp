#include "types.h"
#include "game/event_script.h"

extern const EventScriptCommand data_02169e38[];
s16 ScriptSlot_Find(const EventScriptCommand *script);

/* Hold callback set by EventScript_ScrollMapAndWait: lets the script go on once
 * no slot runs the scroll script data_02169e38 any more. */
void EventScript_ReleaseHoldAfterScroll(EventScriptSlot *slot)
{
    if (ScriptSlot_Find(data_02169e38) == -1) {
        slot->hold = 0;
    }
}
