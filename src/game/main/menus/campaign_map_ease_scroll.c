#include "types.h"
#include "game/campaign_map.h"
#include "game/Unk_02088d04.h"
#include "game/math.h"

void Task_ResumeScript(CampaignMapTask* task);

/* Script step: eases the scroll of the two map layers (the BGs of layers 3 and
 * 2) from (fromX, fromY) to (toX, toY) over 60 frames, then resumes the
 * script. */
void CampaignMap_EaseScroll(CampaignMapTask* task) {
  int layer3Bg;
  int layer2Bg;

  task->step++;
  layer3Bg = data_027c0280->layer3Bg;
  layer2Bg = data_027c0280->layer2Bg;
  data_027c0280->bgScrollX[layer2Bg] = data_027c0280->bgScrollX[layer3Bg] =
      Math_EaseInOut(task->fromX, task->toX, task->step, 60);
  layer3Bg = data_027c0280->layer3Bg;
  layer2Bg = data_027c0280->layer2Bg;
  data_027c0280->bgScrollY[layer2Bg] = data_027c0280->bgScrollY[layer3Bg] =
      Math_EaseInOut(task->fromY, task->toY, task->step, 60);
  if (task->step == 60) {
    task->step = 0;
    Task_ResumeScript(task);
  }
}
