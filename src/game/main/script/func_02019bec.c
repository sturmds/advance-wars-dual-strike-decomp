#include "types.h"
#include "game/event_script.h"

s16 EventScript_FindSlot(const EventScriptCommand* script);
extern const EventScriptCommand data_02158570[];

BOOL func_02019bec(void) { return EventScript_FindSlot(data_02158570) != -1; }
