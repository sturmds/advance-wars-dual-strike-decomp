#include "types.h"
#include "game/task.h"

void Task_ResumeByScript(void* script);

/* The task scripts of the four player slots; VsSetup_DestroyPlayerTasks ends them. */
extern TaskScriptCmd gVsSetupSlot0TaskScript[];
extern TaskScriptCmd gVsSetupSlot1TaskScript[];
extern TaskScriptCmd gVsSetupSlot2TaskScript[];
extern TaskScriptCmd gVsSetupSlot3TaskScript[];

/* Resumes the task of the player's slot (0-3). */
void VsSetup_ResumePlayerTask(int player) {
  switch (player) {
    case 0:
      Task_ResumeByScript(gVsSetupSlot0TaskScript);
      break;
    case 1:
      Task_ResumeByScript(gVsSetupSlot1TaskScript);
      break;
    case 2:
      Task_ResumeByScript(gVsSetupSlot2TaskScript);
      break;
    default:
      Task_ResumeByScript(gVsSetupSlot3TaskScript);
      break;
  }
}
