#include "types.h"
#include "game/campaign_map.h"

extern CampaignMapState gCampaignMapState;

/* Marks no mission as current (28 is one past the last mission) and clears
 * the per-mission flags. */
void CampaignMap_ResetState(void) {
  int i;

  gCampaignMapState.playedMission = 28;
  gCampaignMapState.mission = 0;
  for (i = 0; i < 28; i++) {
    gCampaignMapState.flags[i] = 0;
  }
}
