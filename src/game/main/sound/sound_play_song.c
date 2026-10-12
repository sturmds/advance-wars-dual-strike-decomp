#include "types.h"
#include "game/Unk_02080440.h"

void Sound_RequestSong(
    int songId); /* asks the music task to switch to songId unless it plays already */

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern u16 gCurrentSong; /* song playing, 0xffff: none */
extern u16 gPendingSong; /* song to start later, 0xffff: none */

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
