#include "types.h"
#include "game/song.h"

u16 gPendingSong; /* song to start later, SONG_NONE: none */

void Sound_PlaySong(int songId);

/* Starts the song that Sound_PlaySong kept while the music was not ready, if any. */
void Sound_PlayPendingSong(void) {
  if (gPendingSong != SONG_NONE) {
    Sound_PlaySong(gPendingSong);
  }
}
