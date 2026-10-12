#include "types.h"
#include "game/campaign_map.h"

void CampaignMap_DrawCallout(int x, int y, int side, int step);
void Task_ResumeScript(CampaignMapTask* task);

/* Script step: draws the callout at the mission (fromX, fromY) and counts step
 * down to 0, which shrinks it, then resumes the script. "Callout" is a guess
 * for the scaled sprite CampaignMap_DrawCallout draws. */
void CampaignMap_CalloutShrink(CampaignMapTask* task) {
  CampaignMap_DrawCallout(task->fromX, task->fromY, task->side, task->step);
  if (task->step != 0) {
    task->step--;
    return;
  }
  Task_ResumeScript(task);
}
