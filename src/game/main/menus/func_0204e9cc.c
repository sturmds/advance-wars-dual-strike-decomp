#include "types.h"
#include "game/campaign_map.h"

int CampaignMap_CenterScrollX(int x);
int func_0204ec34(int y);

extern CampaignMapState gCampaignMapState;
extern CampaignMission gCampaignMissions[];

/* Sets up a scroll from the mission just played (gCampaignMapState.unk_00)
 * to the current one: the scroll before and after, the position of the
 * destination, and zeroed counters. */
void func_0204e9cc(CampaignMapTask *task)
{
    s16 fromY;
    int toX;
    int toY;
    u8 from;
    s16 to;
    int fromX;
    to = gCampaignMapState.mission;
    from = gCampaignMapState.unk_00;
    fromX = gCampaignMissions[from].x;
    fromY = gCampaignMissions[from].y;
    toY = gCampaignMissions[to].y;
    toX = gCampaignMissions[to].x;

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
