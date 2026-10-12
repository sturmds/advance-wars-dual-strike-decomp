#include "types.h"
#include "game/campaign_map.h"
#include "game/Unk_02080440.h"

void func_02014c38(void* parentOrList);
void EventScript_Start(const void* script);

extern Unk_02080440* data_027c027c;
extern u32 data_027c0284;
extern CampaignMapState gCampaignMapState;
extern CampaignMission gCampaignMissions[];

/* Starts the event script of the task's mission (CampaignMission.unk_08),
 * loads its map id into the game state and remembers the mission in
 * gCampaignMapState.playedMission. */
void CampaignMap_StartMissionScript(CampaignMapTask* task) {
  func_02014c38(task);
  EventScript_Start(gCampaignMissions[task->mission].unk_08);
  data_027c027c->unk_48[data_027c0284]->unk_1c4->mapId =
      gCampaignMissions[task->mission].mapId;
  gCampaignMapState.playedMission = task->mission;
}
