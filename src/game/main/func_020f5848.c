#include "types.h"
#include "game/event_script.h"

s16 ScriptSlot_Find(const EventScriptCommand* script);
extern const EventScriptCommand gMapScrollScript[];

BOOL func_020f5848(void) { return ScriptSlot_Find(gMapScrollScript) != -1; }
