#include "types.h"
#include "game/song.h"

void Task_DestroyByScript(
    const void* script); /* ends the tasks running script */
void Sound_StartMusicSeq(
    int songId); /* starts songId on the song's sequence player */

/* Starts songId at once, cutting a running fade-out short. */
void Sound_StartSong(int songId) {
  if (gCurrentSong == songId) {
    return;
  }
  Task_DestroyByScript(gSongFadeOutScript);
  Sound_StartMusicSeq(songId);
  gPendingSong = songId;
  gCurrentSong = songId;
}
