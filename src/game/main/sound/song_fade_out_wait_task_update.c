#include "types.h"
#include "game/song.h"

void Task_ResumeScript(void *task);

/* Fades out, waits for the fade to end, then resumes the script unless
 * holdScript is set. */
void SongFadeOutWaitTask_Update(SongFadeOutWaitTask *task)
{
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
