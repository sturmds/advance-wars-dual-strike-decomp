#include "types.h"
#include "game/Unk_02080440.h"
#include "game/campaign_map_music.h"
#include "game/song.h"

/* Starts song 33 in Survival, or when the state has musicReady set. */
void CoSelectScreen_StartSong(void) {
  if (MAP_STATE->gameMode == GAME_MODE_SURVIVAL || MAP_STATE->musicReady != 0) {
    Sound_StartSong(33);
  }
}
