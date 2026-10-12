#include "types.h"
#include "game/Unk_02080440.h"
#include "game/task.h"

extern Unk_02080440* data_027c027c;

/* Pops a task off the free stack. Returns NULL when all TASK_COUNT tasks
 * are in use (the stack ends with a NULL entry). Needed to match: the
 * locals give the post-increment load. */
Task* Task_Alloc(void) {
  Unk_02080440* state = data_027c027c;
  Task** top = state->taskFreeTop;
  Task* task = *top++;

  state->taskFreeTop = top;
  return task;
}
