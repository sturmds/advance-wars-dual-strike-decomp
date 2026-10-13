#include "types.h"
#include "game/campaign_map.h"

CampaignMapState gCampaignMapState;
/* The low byte of CampaignMission.unk_02 of each mission: 2 for the 25th
 * mission (the last story map), 1 for the three after it, else 0. */
extern u8 gCampaignMissionKind[];

/* Whether the current mission is the last story map; Campaign_FinishMap then
 * updates the best average score. */
BOOL CampaignMap_IsFinalMission(void) {
  return (gCampaignMissionKind[gCampaignMapState.playedMission *
                               sizeof(CampaignMission)] &
          2) != 0;
}
