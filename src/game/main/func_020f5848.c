#include "types.h"
#include "game/event_script.h"
#include "game/script_slot.h"

s16 ScriptSlot_Find(const ScriptCommand* script);
extern ScriptCommand gMapScrollScript[];

BOOL func_020f5848(void) { return ScriptSlot_Find(gMapScrollScript) != -1; }
