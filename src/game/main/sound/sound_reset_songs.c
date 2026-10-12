#include "types.h"
#include "game/song.h"

void Sound_SetSeVolume(
    int volume); /* sets the volume of the gSeHandle sound handle */
void Sound_SetMusicVolume(
    int volume); /* sets the volume of the song's handle */

/* Called once from game init: full volume on both handles, no song. */
void Sound_ResetSongs(void) {
  Sound_SetSeVolume(127);
  Sound_SetMusicVolume(127);
  gPendingSong = SONG_NONE;
  gCurrentSong = SONG_NONE;
}
