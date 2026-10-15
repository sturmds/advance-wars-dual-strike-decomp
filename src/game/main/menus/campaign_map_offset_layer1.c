#include "types.h"
#include "game/campaign_map.h"
#include "game/Unk_02088d04.h"

/* Update function: every 4 frames of step, counts the offset unk_64 down
 * (0 to 255, wrapping) and scrolls layer 1 horizontally by it relative to
 * layer 3. The name is a guess. */
void CampaignMap_OffsetLayer1(CampaignMapTask* task) {
  Unk_027c0280* gfx;

  if ((task->step & 3) == 0) {
    task->unk_64--;
    task->unk_64 &= 0xff;
    gfx = data_027c0280;
    gfx->bgScrollX[gfx->layer1Bg] =
        task->unk_64 + gfx->bgScrollX[gfx->layer3Bg];
  }
  task->step++;
}
