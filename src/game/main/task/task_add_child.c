#include "types.h"
#include "game/task.h"

/* Puts task at the head of parent's child list. */
void Task_AddChild(Task* task, Task* parent) {
  if (parent->firstChild != NULL) {
    parent->firstChild->prev = task;
    task->next = parent->firstChild;
  }
  parent->firstChild = task;
  task->parent = parent;
}
