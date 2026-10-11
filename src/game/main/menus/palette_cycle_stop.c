#include "types.h"
#include "game/task.h"

extern const TaskScriptCmd data_02159944[];

void Task_DestroyByScript(const TaskScriptCmd *script);

/* Destroys the task that PaletteCycle_Start created. */
void PaletteCycle_Stop(void)
{
    Task_DestroyByScript(data_02159944);
}
