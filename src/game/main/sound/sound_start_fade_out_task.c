#include "types.h"
#include "game/song.h"
#include "game/task.h"

extern const TaskScriptCmd gSongFadeOutWaitScript[];

Task *Task_Create(const TaskScriptCmd *script, void *parent);

/* Starts a task that fades the song out over fadeLength * 16 frames and
 * waits for the fade to end. */
void Sound_StartFadeOutTask(int holdScript, int fadeLength)
{
    SongFadeOutWaitTask *task = (SongFadeOutWaitTask *)Task_Create(gSongFadeOutWaitScript, (void *)3);

    task->holdScript = holdScript;
    task->fadeLength = fadeLength;
}
