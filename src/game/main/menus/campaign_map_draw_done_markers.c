#include "types.h"
#include "game/campaign_map.h"
#include "game/Unk_02088d04.h"

void CampaignMap_QueueDoneMarker(int x, int y, int frame);

extern CampaignMapState gCampaignMapState;
extern CampaignMission gCampaignMissions[];
extern Unk_027c0280* data_027c0280;

/* Update function: queues a done marker on each mission with the done bit set.
 * The marker animation frame (unk_64) goes 0 to 3, one step every 8 frames of
 * step. */
void CampaignMap_DrawDoneMarkers(CampaignMapTask* task) {
  int i;

  for (i = 0; i < 28; i++) {
    if (gCampaignMapState.flags[i] & 1) {
      CampaignMap_QueueDoneMarker(
          gCampaignMissions[i].x -
              data_027c0280->bgScrollX[data_027c0280->layer3Bg],
          gCampaignMissions[i].y -
              data_027c0280->bgScrollY[data_027c0280->layer3Bg],
          task->unk_64);
    }
  }
  if ((task->step & 7) == 0) {
    task->unk_64++;
    if (task->unk_64 == 4) {
      task->unk_64 = 0;
    }
  }
  task->step++;
}
