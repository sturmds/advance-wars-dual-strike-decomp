#include "types.h"
#include "game/Unk_02080440.h"
#include "game/song.h"

void Sound_RequestSong(
    int songId); /* asks the music task to switch to songId unless it plays already */

/* Starts song songId. While musicReady of the current state is 0 the song
 * is only kept as pending, and Sound_PlayPendingSong starts it later. */
void Sound_PlaySong(int songId) {
  if (data_027c027c->unk_48[data_027c0284]->musicReady != 0) {
    Sound_RequestSong(songId);
    return;
  }
  gPendingSong = songId;
  gCurrentSong = 0xffff;
}
