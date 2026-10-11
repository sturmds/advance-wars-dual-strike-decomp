#include "types.h"
#include "game/task.h"

void Task_DestroyByScript(const TaskScriptCmd *script);

/* One task script per player slot; VsSetup_ResumePlayerTask resumes them by player. */
extern const TaskScriptCmd gVsSetupSlot0TaskScript[];
extern const TaskScriptCmd gVsSetupSlot1TaskScript[];
extern const TaskScriptCmd gVsSetupSlot2TaskScript[];
extern const TaskScriptCmd gVsSetupSlot3TaskScript[];

void VsSetup_DestroyPlayerTasks(void)
{
    Task_DestroyByScript(gVsSetupSlot0TaskScript);
    Task_DestroyByScript(gVsSetupSlot1TaskScript);
    Task_DestroyByScript(gVsSetupSlot2TaskScript);
    Task_DestroyByScript(gVsSetupSlot3TaskScript);
}
