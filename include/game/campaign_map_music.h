#include "game/song.h"
#ifndef GAME_CAMPAIGN_MAP_MUSIC_H
#define GAME_CAMPAIGN_MAP_MUSIC_H

#include "types.h"
#include "game/campaign_map.h"

extern CampaignMapState gCampaignMapState;

/* Starts the world map song of the selected mission: song 6 for missions 0
 * to 8, song 48 for 19 to 24, song 42 for the others. */
static inline void CampaignMap_StartMissionSong(void) {
  if (gCampaignMapState.mission <= 8) {
    Sound_StartSong(6);
  } else if (gCampaignMapState.mission <= 18 ||
             gCampaignMapState.mission >= 25) {
    Sound_StartSong(42);
  } else {
    Sound_StartSong(48);
  }
}

#endif
