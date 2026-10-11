#include "types.h"

void Task_DestroyByScript(void *script);

/* One task script per player slot; func_0206b6c4 resumes them by player. */
extern u32 data_0215cec8[];
extern u32 data_0215cf00[];
extern u32 data_0215cf38[];
extern u32 data_0215cf70[];

void VsSetup_DestroyPlayerTasks(void)
{
    Task_DestroyByScript(data_0215cec8);
    Task_DestroyByScript(data_0215cf00);
    Task_DestroyByScript(data_0215cf38);
    Task_DestroyByScript(data_0215cf70);
}
