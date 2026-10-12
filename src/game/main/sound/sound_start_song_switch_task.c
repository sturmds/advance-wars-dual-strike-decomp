#include "types.h"
#include "game/song.h"
#include "game/task.h"

extern const TaskScriptCmd gSongSwitchScript[];

/* Starts a task that fades the current song out over fadeLength * 16
 * frames and then starts songId. */
SongSwitchTask* Sound_StartSongSwitchTask(int songId, int holdScript,
                                          int fadeLength) {
  SongSwitchTask* task =
      (SongSwitchTask*)Task_Create(gSongSwitchScript, TASK_LIST(3));

  task->songId = songId;
  task->holdScript = holdScript;
  task->fadeLength = fadeLength;
  return task;
}
