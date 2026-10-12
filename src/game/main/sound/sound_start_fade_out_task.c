#include "types.h"
#include "game/song.h"
#include "game/task.h"

extern const TaskScriptCmd gSongFadeOutWaitScript[];

/* Starts a task that fades the song out over fadeLength * 16 frames and
 * waits for the fade to end. */
void Sound_StartFadeOutTask(int holdScript, int fadeLength) {
  SongFadeOutWaitTask* task =
      (SongFadeOutWaitTask*)Task_Create(gSongFadeOutWaitScript, TASK_LIST(3));

  task->holdScript = holdScript;
  task->fadeLength = fadeLength;
}
