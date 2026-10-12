#include "types.h"
#include "game/task.h"
#include "game/vs_setup_slot_task.h"

/* The task scripts of the four player slots. */
extern const TaskScriptCmd gVsSetupSlot0TaskScript[];
extern const TaskScriptCmd gVsSetupSlot1TaskScript[];
extern const TaskScriptCmd gVsSetupSlot2TaskScript[];
extern const TaskScriptCmd gVsSetupSlot3TaskScript[];

/* Starts the task of a player slot (0-3) in task list 3 at pixel (x, y). */
void VsSetup_StartPlayerTask(int slot, u16 x, int y) {
  VsSetupSlotTask* task;

  switch (slot) {
    case 0:
      task =
          (VsSetupSlotTask*)Task_Create(gVsSetupSlot0TaskScript, TASK_LIST(3));
      break;
    case 1:
      task =
          (VsSetupSlotTask*)Task_Create(gVsSetupSlot1TaskScript, TASK_LIST(3));
      break;
    case 2:
      task =
          (VsSetupSlotTask*)Task_Create(gVsSetupSlot2TaskScript, TASK_LIST(3));
      break;
    default:
      task =
          (VsSetupSlotTask*)Task_Create(gVsSetupSlot3TaskScript, TASK_LIST(3));
      break;
  }
  task->x = x;
  task->y = y;
  task->matrix = slot + 25;
}
