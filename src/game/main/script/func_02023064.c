#include "types.h"
#include "game/task.h"

void Sound_PlaySe(int seId);

/* Task script command: plays the sound effect named by the command's value
 * and moves on to the next command. */
BOOL func_02023064(Task* task) {
  Sound_PlaySe(task->pc->value);
  task->pc++;
  return TRUE;
}
