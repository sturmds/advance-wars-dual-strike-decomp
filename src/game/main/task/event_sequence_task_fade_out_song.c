#include "types.h"
#include "game/song.h"
#include "game/task.h"

/* Script step of the task that EventSequenceTask_Start creates (script
 * data_021676a0): fades the song out when unk_66 is nonzero. */
void EventSequenceTask_FadeOutSong(Task* task) {
  if (task->unk_66 == 0) {
    return;
  }
  Sound_FadeOutSong();
}
