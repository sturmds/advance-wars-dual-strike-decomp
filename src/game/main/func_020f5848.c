#include "types.h"
#include "game/event_script.h"

s16 ScriptSlot_Find(const EventScriptCommand *script);
extern const EventScriptCommand data_02169e38[];

BOOL func_020f5848(void)
{
    return ScriptSlot_Find(data_02169e38) != -1;
}
