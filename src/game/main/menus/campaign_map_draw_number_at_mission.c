#include "types.h"
#include "game/campaign_map.h"
#include "game/Unk_02088d04.h"

void func_0204f450(int x, int y, int side);

extern Unk_027c0280 *data_027c0280;

/* Draws func_0204f450's number sprites (digits of func_020d61e8, a guess from
 * its use of divide and modulo by 10) at the task's mission position, minus the map scroll. */
void CampaignMap_DrawNumberAtMission(CampaignMapTask *task)
{
    Unk_027c0280 *gfx;

    gfx = data_027c0280;
    func_0204f450(task->fromX - gfx->bgScrollX[gfx->layer3Bg], task->fromY - gfx->bgScrollY[gfx->layer3Bg], task->side);
}
