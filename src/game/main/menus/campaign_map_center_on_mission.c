#include "types.h"
#include "game/campaign_map.h"
#include "game/Unk_02088d04.h"
#include "game/task.h"
#include "game/campaign_map_music.h"

int CampaignMap_CenterScrollX(int x);
int func_0204ec34(int y);
void Task_ResumeByScript(const void* script);

extern CampaignMission gCampaignMissions[];
extern TaskScriptCmd data_0215b420[];

/* Script step: sets the scroll of the two map layers (the BGs of layers 3 and
 * 2) so the current mission is centred, then resumes the script by
 * data_0215b420. */
void CampaignMap_CenterOnMission(void) {
  int layer3Bg;
  int layer2Bg;

  layer3Bg = data_027c0280->layer3Bg;
  layer2Bg = data_027c0280->layer2Bg;
  data_027c0280->bgScrollX[layer2Bg] = data_027c0280->bgScrollX[layer3Bg] =
      CampaignMap_CenterScrollX(gCampaignMissions[gCampaignMapState.mission].x);
  layer3Bg = data_027c0280->layer3Bg;
  layer2Bg = data_027c0280->layer2Bg;
  data_027c0280->bgScrollY[layer2Bg] = data_027c0280->bgScrollY[layer3Bg] =
      func_0204ec34(gCampaignMissions[gCampaignMapState.mission].y);
  Task_ResumeByScript(data_0215b420);
}
