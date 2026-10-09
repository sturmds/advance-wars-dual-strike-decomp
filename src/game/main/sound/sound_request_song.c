#include "types.h"
#include "game/song.h"

void *Task_Create(const void *script, int group);

/* Starts a task that switches the music to songId once the current song
 * has faded out. Does nothing if songId already plays. */
void Sound_RequestSong(int songId)
{
    SongRequestTask *task;

    if (gCurrentSong == songId) {
        return;
    }
    task = Task_Create(gSongRequestScript, 3);
    task->songId = songId;
}
