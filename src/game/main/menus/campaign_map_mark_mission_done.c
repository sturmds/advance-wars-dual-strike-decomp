#include "types.h"
#include "game/campaign_map.h"

void CampaignMap_PickNextMission(void);

extern CampaignMapState gCampaignMapState;
extern u8 data_0229c74a[];

/* Sets bit 0 of the flags of the mission just played (unk_00), then picks
 * the next mission (CampaignMap_PickNextMission writes CampaignMapState.mission). */
void CampaignMap_MarkMissionDone(void) {
  data_0229c74a[gCampaignMapState.playedMission] |= 1;
  CampaignMap_PickNextMission();
}
