#include "types.h"
#include "game/campaign_map.h"
#include "game/Unk_02088d04.h"
#include "game/math.h"

void CampaignMap_DrawNumberTag(int x, int y, int side);
void Task_ResumeScript(CampaignMapTask* task);

/* Script step: like CampaignMap_DrawNumberAtMission, with the sprites sliding
 * in over 16 frames from 256 pixels to the right (side 0) or left; then
 * resumes the script. */
void CampaignMap_SlideInNumber(CampaignMapTask* task) {
  Unk_027c0280* gfx;
  int slide;

  task->step++;
  if (task->side != 0) {
    slide = Math_Ease(4, -256, 0, task->step, 16);
  } else {
    slide = Math_Ease(4, 256, 0, task->step, 16);
  }
  gfx = data_027c0280;
  CampaignMap_DrawNumberTag(
      slide + (task->fromX - gfx->bgScrollX[gfx->layer3Bg]),
      task->fromY - gfx->bgScrollY[gfx->layer3Bg], task->side);
  if (task->step != 16) {
    return;
  }
  task->step = 0;
  Task_ResumeScript(task);
}
