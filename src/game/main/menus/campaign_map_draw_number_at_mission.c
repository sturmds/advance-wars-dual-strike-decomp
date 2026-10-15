#include "types.h"
#include "game/campaign_map.h"
#include "game/Unk_02088d04.h"

void CampaignMap_DrawNumberTag(int x, int y, int side);

/* Draws the number tag (CampaignMap_DrawNumberTag) at the task's mission
 * position, minus the map scroll. */
void CampaignMap_DrawNumberAtMission(CampaignMapTask* task) {
  Unk_027c0280* gfx;

  gfx = data_027c0280;
  CampaignMap_DrawNumberTag(task->fromX - gfx->bgScrollX[gfx->layer3Bg],
                            task->fromY - gfx->bgScrollY[gfx->layer3Bg],
                            task->side);
}
