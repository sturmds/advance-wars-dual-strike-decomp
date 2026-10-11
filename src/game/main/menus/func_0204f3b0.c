#include "types.h"
#include "game/campaign_map.h"

int CampaignMap_CenterScrollX(int x);

extern CampaignMapState gCampaignMapState;
extern CampaignMission gCampaignMissions[];

#define MISSION gCampaignMissions[gCampaignMapState.mission]

/* Script step: sets up the callout at the current mission: side 1 when the
 * mission lies right of the middle of the screen, its position, step 0. The
 * original stores step last and still reloads the mission index between the
 * two position stores; storing step between them gives the reload (91.6%). */
void func_0204f3b0(CampaignMapTask *task)
{
    if (MISSION.x - CampaignMap_CenterScrollX(MISSION.x) > 120) {
        task->side = 1;
    } else {
        task->side = 0;
    }
    task->fromX = MISSION.x;
    task->step = 0;
    task->fromY = MISSION.y;
}
