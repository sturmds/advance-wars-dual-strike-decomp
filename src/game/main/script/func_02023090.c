#include "types.h"
#include "game/task.h"
#include "game/song.h"

/* Task script command: starts the song named by the command's value and
 * moves on to the next command. */
BOOL func_02023090(Task* task) {
  Sound_StartSong(task->pc->value);
  task->pc++;
  return TRUE;
}
