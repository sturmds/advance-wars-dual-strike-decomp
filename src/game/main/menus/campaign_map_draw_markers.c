#include "types.h"
#include "game/campaign_map.h"
#include "game/Unk_02088d04.h"
#include "game/campaign_map_music.h"

void CampaignMap_QueueDoneMarker(int x, int y, int frame);
void CampaignMap_QueueCurrentMarker(int x, int y, int frame);

extern CampaignMission gCampaignMissions[];

/* Update function: queues a done marker on each mission with the done bit set
 * and the current marker on the current mission if it is not done. The marker
 * animation frame (unk_64) goes 0 to 3, one step every 8 frames of step. */
void CampaignMap_DrawMarkers(CampaignMapTask* task) {
  int i;

  for (i = 0; i < CAMPAIGN_MISSION_COUNT; i++) {
    if (gCampaignMapState.flags[i] & 1) {
      CampaignMap_QueueDoneMarker(
          gCampaignMissions[i].x -
              data_027c0280->bgScrollX[data_027c0280->layer3Bg],
          gCampaignMissions[i].y -
              data_027c0280->bgScrollY[data_027c0280->layer3Bg],
          task->unk_64);
    } else if (i == gCampaignMapState.mission) {
      CampaignMap_QueueCurrentMarker(
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
