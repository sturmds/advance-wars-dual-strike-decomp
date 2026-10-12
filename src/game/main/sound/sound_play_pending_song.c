#include "types.h"
#include "game/song.h"

void Sound_PlaySong(int songId);

/* Starts the song that Sound_PlaySong kept while the music was not ready, if any. */
void Sound_PlayPendingSong(void) {
  if (gPendingSong != SONG_NONE) {
    Sound_PlaySong(gPendingSong);
  }
}
