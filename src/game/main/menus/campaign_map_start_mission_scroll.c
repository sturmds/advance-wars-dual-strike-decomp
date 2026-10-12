/* Needed to match: the original reads gCampaignMapState.mission again at the
 * end, which mwcc only does with common subexpression removal off. */
#pragma opt_common_subs off

#include "types.h"
#include "game/campaign_map.h"

int CampaignMap_CenterScrollX(int x);
int func_0204ec34(int y);

extern CampaignMapState gCampaignMapState;
extern CampaignMission gCampaignMissions[];

/* Sets up a scroll from the mission just played (gCampaignMapState.playedMission)
 * to the current one: the scroll before and after, the position of the
 * destination, and zeroed counters. */
void CampaignMap_StartMissionScroll(CampaignMapTask* task) {
  s16 fromY;
  int toX;
  int toY;
  u8 from;
  int to;
  int fromX;

  from = gCampaignMapState.playedMission;
  fromX = gCampaignMissions[from].x;
  fromY = gCampaignMissions[from].y;
  to = gCampaignMapState.mission;
  toX = gCampaignMissions[to].x;
  toY = gCampaignMissions[to].y;

  task->unk_64 = 0;
  task->unk_68 = 0;
  task->step = 0;
  task->fromX = CampaignMap_CenterScrollX(fromX);
  task->fromY = func_0204ec34(fromY);
  task->cursorX = toX;
  task->cursorY = toY;
  task->toX = CampaignMap_CenterScrollX(toX);
  task->toY = func_0204ec34(toY);
  task->mission = gCampaignMapState.mission;
}
