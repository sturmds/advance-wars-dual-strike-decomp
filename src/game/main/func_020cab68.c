#include "types.h"
#include "game/script_slot.h"

void ScriptSlot_Start(const void* data, int arg);

extern ScriptCommand data_02166490[];

void func_020cab68(void) { ScriptSlot_Start(data_02166490, 0); }
