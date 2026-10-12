#include "types.h"
#include "game/campaign_map.h"

void CampaignMap_DrawCallout(int x, int y, int side, int step);
void Task_ResumeScript(CampaignMapTask* task);
void Sound_PlaySe(u32 soundId);

/* Script step: draws the callout growing as step counts 0 to 16. At 16 it
 * plays sound 70 (side 0) or 71, resets step and resumes the script. */
void CampaignMap_CalloutGrow(CampaignMapTask* task) {
  CampaignMap_DrawCallout(task->fromX, task->fromY, task->side, task->step);
  if (task->step == 16) {
    if (task->side == 0) {
      Sound_PlaySe(70);
    } else {
      Sound_PlaySe(71);
    }
    task->step = 0;
    Task_ResumeScript(task);
    return;
  }
  task->step++;
}
