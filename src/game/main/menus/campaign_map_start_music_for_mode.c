#include "types.h"
#include "game/campaign_map_music.h"

BOOL Player1Team_HasPlayerInGame(void);

/* Like CampaignMap_StartMusic, but when the world map returns from a battle
 * (mode 2, see CampaignMap_StartAfterBattle) and the mission is 9 or 19, the
 * first mission of a new song range, it leaves the music alone (guess: the
 * scene that follows plays its own). */
void CampaignMap_StartMusicForMode(Unk_0204f960* task) {
  if (Player1Team_HasPlayerInGame() && task->unk_52 == 2) {
    if (gCampaignMapState.mission == 9) {
      return;
    }
    if (gCampaignMapState.mission == 19) {
      return;
    }
  }
  CampaignMap_StartMissionSong();
}
