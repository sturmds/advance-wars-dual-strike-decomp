#include "types.h"
#include "game/campaign_map.h"
#include "game/task.h"
#include "game/Unk_02088d04.h"
#include "game/campaign_map_music.h"

extern CampaignMission gCampaignMissions[];
extern TaskScriptCmd data_0215b470[];

/* Starts the task of script data_0215b470 for the mission just played
 * (gCampaignMapState.playedMission). It gets side 0 and the mission's position on
 * the screen (map position minus the layer 3 scroll) in fromX and fromY. */
void CampaignMap_StartPlayedMissionTask(void) {
  CampaignMapTask* task =
      (CampaignMapTask*)Task_Create(data_0215b470, TASK_LIST(3));

  task->side = 0;
  task->fromX = gCampaignMissions[gCampaignMapState.playedMission].x -
                data_027c0280->bgScrollX[data_027c0280->layer3Bg];
  task->fromY = gCampaignMissions[gCampaignMapState.playedMission].y -
                data_027c0280->bgScrollY[data_027c0280->layer3Bg];
}
