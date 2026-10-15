#include "types.h"
#include "game/campaign_map.h"
#include "game/Unk_02080440.h"
#include "game/campaign_map_music.h"

void func_02014c38(void* parentOrList);
void EventScript_Start(const void* script);

extern CampaignMission gCampaignMissions[];

/* Starts the event script of the task's mission (CampaignMission.script),
 * loads its map id into the game state and remembers the mission in
 * gCampaignMapState.playedMission. */
void CampaignMap_StartMissionScript(CampaignMapTask* task) {
  func_02014c38(task);
  EventScript_Start(gCampaignMissions[task->mission].script);
  data_027c027c->unk_48[data_027c0284]->unk_1c4->mapId =
      gCampaignMissions[task->mission].mapId;
  gCampaignMapState.playedMission = task->mission;
}
