#include "types.h"
#include "game/song.h"

void Task_ResumeScript(void* task);

/* Fades out, waits, starts the new song; with holdScript set it then keeps
 * running instead of resuming the script. */
void SongSwitchTask_Update(SongSwitchTask* task) {
  switch (task->step) {
    case 0:
      Sound_FadeOutSongOver(task->fadeLength);
      task->step++;
    case 1:
      if (!Sound_IsSongFadingOut()) {
        task->step++;
      }
      return;
    case 2:
      Sound_StartSong(task->songId);
      task->unk_30 = 0;
      task->step++;
    case 3:
      if (task->holdScript != 0) {
        return;
      }
      Task_ResumeScript(task);
      break;
  }
}
