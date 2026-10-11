#include "types.h"
#include "game/task.h"
#include "game/combat_setup_task.h"

extern const TaskScriptCmd gCombatSetupTaskScript[];
extern u32 data_0229eaec;

/* Starts the Combat setup task with default settings (unk_4e clear) and sets data_0229eaec, which
 * looks like a link play flag: the army variant readers and the battle save check it. */
void CombatSetup_StartLink(void)
{
    CombatSetupTask *task = (CombatSetupTask *)Task_Create(gCombatSetupTaskScript, TASK_LIST(3));

    task->unk_4e = 0;
    data_0229eaec = 1;
}
