#include "types.h"

extern u8 data_022900b8;
extern u8 data_021584f8[];
extern u8 data_02158520[];

void ScriptSlot_StopByScript(const void *script);

void func_020154c4(void)
{
    if (data_022900b8 == 2) {
        data_022900b8 = 0;
    }
    ScriptSlot_StopByScript(data_021584f8);
    ScriptSlot_StopByScript(data_02158520);
}
