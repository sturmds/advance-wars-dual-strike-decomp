#include "types.h"
#include "game/task.h"

extern const TaskScriptCmd data_021676a0[];

/* Creates a child task of parent that runs script data_021676a0 (set up, event
 * script, song fade-out step). The task's flag unk_66 is 1, so the song fades out at the end. */
void EventSequenceTask_StartWithSongFade(Task* parent) {
  Task* task = Task_CreateChild(data_021676a0, parent);

  task->unk_64 = 0;
  task->unk_66 = 1;
  task->unk_4c = NULL;
}
