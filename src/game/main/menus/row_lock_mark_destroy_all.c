#include "types.h"
#include "game/task.h"

void Task_DestroyByScript(const TaskScriptCmd *script);
extern const TaskScriptCmd gRowLockMarkRow1Script[];
extern const TaskScriptCmd gRowLockMarkRow2Script[];
extern const TaskScriptCmd gRowLockMarkRow3Script[];
extern const TaskScriptCmd gRowLockMarkRow4Script[];

void RowLockMark_DestroyAll(void)
{
    Task_DestroyByScript(gRowLockMarkRow1Script);
    Task_DestroyByScript(gRowLockMarkRow2Script);
    Task_DestroyByScript(gRowLockMarkRow3Script);
    Task_DestroyByScript(gRowLockMarkRow4Script);
}
