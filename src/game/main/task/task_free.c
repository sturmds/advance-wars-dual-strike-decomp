#include "types.h"
#include "game/Unk_02080440.h"
#include "game/task.h"

/* Pushes task back onto the free stack. */
void Task_Free(Task* task) {
  data_027c027c->taskFreeTop--;
  *data_027c027c->taskFreeTop = task;
}
