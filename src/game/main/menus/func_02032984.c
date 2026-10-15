#include "types.h"
#include "game/campaign_map_music.h"
#include "game/song.h"

BOOL CoExp_IsSpecialMission(void);

void func_02032984(void) {
  if (CoExp_IsSpecialMission()) {
    Sound_StartSong(21);
  } else {
    Sound_StartSong(20);
  }
}
