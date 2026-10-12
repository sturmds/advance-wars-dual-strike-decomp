#include "types.h"
#include "game/campaign_map.h"
#include "game/task.h"
#include "game/Unk_02088d04.h"

extern CampaignMapState gCampaignMapState;
extern CampaignMission gCampaignMissions[];
extern Unk_027c0280* data_027c0280;
extern TaskScriptCmd data_0215b4a0[];

/* Same as CampaignMap_StartPlayedMissionTask for the next mission
 * (gCampaignMapState.mission), script data_0215b4a0 and side 1. */
void CampaignMap_StartNextMissionTask(void) {
  CampaignMapTask* task =
      (CampaignMapTask*)Task_Create(data_0215b4a0, TASK_LIST(3));

  task->side = 1;
  task->fromX = gCampaignMissions[gCampaignMapState.mission].x -
                data_027c0280->bgScrollX[data_027c0280->layer3Bg];
  task->fromY = gCampaignMissions[gCampaignMapState.mission].y -
                data_027c0280->bgScrollY[data_027c0280->layer3Bg];
}
