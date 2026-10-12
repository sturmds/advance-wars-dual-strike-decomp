#include "types.h"
#include "game/campaign_map.h"
#include "game/unlocks.h"

extern CampaignMapState gCampaignMapState;

/* Sets the mission that follows the one just played (unk_00): usually the
 * next one. After missions 9, 11 and 13 the side missions 25, 26 and 27
 * come first when game flags 0x60, 0x61 and 0x62 are set, and each side
 * mission leads back to 10, 12 and 14. After mission 24 it is selected again. */
void CampaignMap_PickNextMission(void) {
  switch (gCampaignMapState.playedMission) {
    case 9:
      if (GameFlag_IsSet(0x60)) {
        gCampaignMapState.mission = 25;
      } else {
        gCampaignMapState.mission = gCampaignMapState.playedMission + 1;
      }
      break;
    case 11:
      if (GameFlag_IsSet(0x61)) {
        gCampaignMapState.mission = 26;
      } else {
        gCampaignMapState.mission = gCampaignMapState.playedMission + 1;
      }
      break;
    case 13:
      if (GameFlag_IsSet(0x62)) {
        gCampaignMapState.mission = 27;
      } else {
        gCampaignMapState.mission = gCampaignMapState.playedMission + 1;
      }
      break;
    case 25:
      gCampaignMapState.mission = 10;
      break;
    case 26:
      gCampaignMapState.mission = 12;
      break;
    case 27:
      gCampaignMapState.mission = 14;
      break;
    case 24:
      gCampaignMapState.mission = 24;
      break;
    default:
      gCampaignMapState.mission = gCampaignMapState.playedMission + 1;
      break;
  }
}
