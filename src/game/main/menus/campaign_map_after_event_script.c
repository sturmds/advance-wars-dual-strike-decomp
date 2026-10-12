#include "types.h"
#include "game/campaign_map.h"

BOOL EventScript_IsRunning(void);
int func_020b983c(void);
void func_0204d8c8(void);
void CampaignMap_AddMissionButtons(void);
void Task_ResumeScript(CampaignMapTask* task);
void Task_JumpToLabel(void* task, int label);

/* Script step: waits for the event script to end. If func_020b983c (a byte
 * of the current screen state) is nonzero, the task goes back to label 0 and
 * the mission buttons come back; otherwise the script goes on after a task
 * made by func_0204d8c8. */
void CampaignMap_AfterEventScript(CampaignMapTask* task) {
  if (EventScript_IsRunning()) {
    return;
  }
  if (func_020b983c() == 0) {
    func_0204d8c8();
    Task_ResumeScript(task);
  } else {
    Task_JumpToLabel(task, 0);
    CampaignMap_AddMissionButtons();
  }
}
