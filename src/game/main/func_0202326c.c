#include "types.h"
#include "game/task.h"
#include "game/song.h"

BOOL func_0202326c(Task* task) {
  Sound_FadeOutSongOver(task->pc->arg);
  task->pc++;
  return TRUE;
}
