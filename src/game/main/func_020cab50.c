#include "types.h"
#include "game/script_slot.h"

void ScriptSlot_Start(const void* data, int arg);
extern ScriptCommand data_02166508[];

void func_020cab50(void) { ScriptSlot_Start(data_02166508, 0); }
