#include "types.h"
#include "game/song.h"

/* Sound_StartSongSwitchTask with a 32-frame fade. */
SongSwitchTask* Sound_StartSongSwitch(int songId, int holdScript) {
  return Sound_StartSongSwitchTask(songId, holdScript, 2);
}
