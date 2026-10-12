#include "types.h"
#include "game/campaign_map.h"

void CampaignMap_StartPlayedMissionTask(void);
BOOL func_0204ec78(void);
void Task_ResumeScript(CampaignMapTask* task);

/* Script step: starts the task of CampaignMap_StartPlayedMissionTask on the first call, then
 * waits while func_0204ec78 finds one of the tasks of data_0215b470 or
 * data_0215b4a0, and resumes the script. */
void CampaignMap_StartTaskAndWait(CampaignMapTask* task) {
  if (task->step == 0) {
    CampaignMap_StartPlayedMissionTask();
    task->step++;
    return;
  }
  if (func_0204ec78()) {
    return;
  }
  task->step = 0;
  Task_ResumeScript(task);
}
