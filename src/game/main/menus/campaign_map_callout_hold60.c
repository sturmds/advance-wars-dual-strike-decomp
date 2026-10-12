#include "types.h"
#include "game/campaign_map.h"

void CampaignMap_DrawCallout(int x, int y, int side, int step);
void Task_ResumeScript(CampaignMapTask* task);

/* Script step: draws the callout at full size (step argument -1) for 60
 * frames, then sets step to 16 for the shrink step and resumes the script. */
void CampaignMap_CalloutHold60(CampaignMapTask* task) {
  CampaignMap_DrawCallout(task->fromX, task->fromY, task->side, -1);
  if (task->step != 60) {
    task->step++;
    return;
  }
  task->step = 16;
  Task_ResumeScript(task);
}
