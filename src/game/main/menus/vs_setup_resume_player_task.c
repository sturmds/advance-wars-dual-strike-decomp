#include "types.h"

void Task_ResumeByScript(void* script);

/* The task scripts of the four player slots; VsSetup_DestroyPlayerTasks ends them. */
extern u32 gVsSetupSlot0TaskScript[];
extern u32 gVsSetupSlot1TaskScript[];
extern u32 gVsSetupSlot2TaskScript[];
extern u32 gVsSetupSlot3TaskScript[];

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
