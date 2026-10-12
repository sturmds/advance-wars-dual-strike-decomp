#include "types.h"
#include "game/campaign_map.h"

void CampaignMap_AddMissionButtons(void);
void Task_ResumeScript(CampaignMapTask* task);

/* Script step: waits 16 frames, adds the mission buttons and resumes the
 * script. */
void CampaignMap_WaitThenAddButtons(CampaignMapTask* task) {
  task->step++;
  if (task->step != 16) {
    return;
  }
  task->step = 0;
  CampaignMap_AddMissionButtons();
  Task_ResumeScript(task);
}
