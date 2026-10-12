#include "types.h"
#include "game/task.h"

void Task_RunTree(Task* task);

/* Runs one frame of every task in a list (head: a taskLists entry) and
 * of their children. */
void Task_RunList(Task* task) {
  if (task != NULL) {
    Task_RunTree(task);
  }
}
