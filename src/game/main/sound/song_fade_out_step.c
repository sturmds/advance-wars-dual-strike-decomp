#include "types.h"
#include "game/song.h"

void Task_ResumeScript(void* task); /* ends task */

/* Per-frame step of gSongFadeOutScript: ends the task when the count
 * reaches 0. */
void SongFadeOut_Step(SongFadeOutTask* task) {
  task->framesLeft--;
  if (task->framesLeft != 0) {
    return;
  }
  Task_ResumeScript(task);
}
