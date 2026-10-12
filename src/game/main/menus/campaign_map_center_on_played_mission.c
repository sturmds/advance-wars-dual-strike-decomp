#include "types.h"
#include "game/campaign_map.h"
#include "game/Unk_02088d04.h"

int CampaignMap_CenterScrollX(int x);
int func_0204ec34(int y);

extern CampaignMapState gCampaignMapState;
extern CampaignMission gCampaignMissions[];
extern Unk_027c0280* data_027c0280;

/* Like CampaignMap_CenterOnMission, but for the mission just played
 * (gCampaignMapState.playedMission, the one CampaignMap_MarkMissionDone flags), and
 * it does not resume a script. */
void CampaignMap_CenterOnPlayedMission(void) {
  int layer3Bg;
  int layer2Bg;

  layer3Bg = data_027c0280->layer3Bg;
  layer2Bg = data_027c0280->layer2Bg;
  data_027c0280->bgScrollX[layer2Bg] = data_027c0280->bgScrollX[layer3Bg] =
      CampaignMap_CenterScrollX(
          gCampaignMissions[gCampaignMapState.playedMission].x);
  layer3Bg = data_027c0280->layer3Bg;
  layer2Bg = data_027c0280->layer2Bg;
  data_027c0280->bgScrollY[layer2Bg] = data_027c0280->bgScrollY[layer3Bg] =
      func_0204ec34(gCampaignMissions[gCampaignMapState.playedMission].y);
}
