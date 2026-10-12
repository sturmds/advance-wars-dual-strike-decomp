#include "types.h"
#include "game/task.h"
#include "game/song.h"

/* Starts a task that switches the music to songId once the current song
 * has faded out. Does nothing if songId already plays. */
void Sound_RequestSong(int songId) {
  SongRequestTask* task;

  if (gCurrentSong == songId) {
    return;
  }
  task = (SongRequestTask*)Task_Create(gSongRequestScript, TASK_LIST(3));
  task->songId = songId;
}
