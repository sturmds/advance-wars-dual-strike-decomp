#include "types.h"
#include "game/task.h"
#include "game/vs_setup_slot_task.h"

/* The task scripts of the four player slots. */
extern const TaskScriptCmd data_0215cec8[];
extern const TaskScriptCmd data_0215cf00[];
extern const TaskScriptCmd data_0215cf38[];
extern const TaskScriptCmd data_0215cf70[];

/* Starts the task of a player slot (0-3) in task list 3 at pixel (x, y). */
void VsSetup_StartPlayerTask(int slot, u16 x, int y)
{
    VsSetupSlotTask *task;

    switch (slot) {
    case 0:
        task = (VsSetupSlotTask *)Task_Create(data_0215cec8, TASK_LIST(3));
        break;
    case 1:
        task = (VsSetupSlotTask *)Task_Create(data_0215cf00, TASK_LIST(3));
        break;
    case 2:
        task = (VsSetupSlotTask *)Task_Create(data_0215cf38, TASK_LIST(3));
        break;
    default:
        task = (VsSetupSlotTask *)Task_Create(data_0215cf70, TASK_LIST(3));
        break;
    }
    task->x = x;
    task->y = y;
    task->id = slot + 25;
}
