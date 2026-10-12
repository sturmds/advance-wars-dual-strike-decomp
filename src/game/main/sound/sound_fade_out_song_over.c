#include "types.h"
#include "game/task.h"
#include "game/song.h"

/* Sets the song volume to from (-1: keep the current volume), then fades
 * it to to over frames frames. */
void Sound_FadeMusicVolume(int from, int to, int frames);

/* Fades the song from 127 to 0 over length * 16 frames (length 0: 2) and
 * starts a task that clears gCurrentSong when the time is up. */
void Sound_FadeOutSongOver(int length) {
  int frames;
  SongFadeOutTask* task;

  if (gCurrentSong == SONG_NONE) {
    return;
  }
  if (length == 0) {
    length = 2;
  }
  frames = length << 4;
  Sound_FadeMusicVolume(127, 0, frames);
  task = (SongFadeOutTask*)Task_Create(gSongFadeOutScript, TASK_LIST(3));
  task->framesLeft = frames;
}
