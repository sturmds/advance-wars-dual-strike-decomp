#include "types.h"
#include "game/task.h"

void *Task_FindByScript(const TaskScriptCmd *script);
extern TaskScriptCmd gBattleRecordTaskScript[];

BOOL BattleRecordTask_IsRunning(void)
{
    return Task_FindByScript(gBattleRecordTaskScript) != NULL;
}
