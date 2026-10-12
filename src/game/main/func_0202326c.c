#include "types.h"
#include "game/task.h"

void Sound_FadeOutSongOver(int arg);

BOOL func_0202326c(Task* task) {
  Sound_FadeOutSongOver(task->pc->arg);
  task->pc++;
  return TRUE;
}
