#include "types.h"
#include "game/script_slot.h"

extern ScriptCommand data_021601d0[];
extern ScriptCommand data_02160208[];
extern ScriptCommand data_02160240[];
void Task_DestroyByScript(const ScriptCommand *script);

void func_0207fc50(void)
{
    Task_DestroyByScript(data_021601d0);
    Task_DestroyByScript(data_02160208);
    Task_DestroyByScript(data_02160240);
}
