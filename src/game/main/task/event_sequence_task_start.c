#include "types.h"
#include "game/task.h"

extern const TaskScriptCmd data_021676a0[];

/* Like EventSequenceTask_StartWithSongFade, but the song keeps playing
 * (flag unk_66 is 0). */
void EventSequenceTask_Start(Task* parent) {
  Task* task = Task_CreateChild(data_021676a0, parent);

  task->unk_64 = 0;
  task->unk_66 = 0;
  task->unk_4c = NULL;
}
