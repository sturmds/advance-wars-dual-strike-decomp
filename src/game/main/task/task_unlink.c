#include "types.h"
#include "game/Unk_02080440.h"
#include "game/task.h"

extern Unk_02080440* data_027c027c;

/* Removes task from its parent's child list or from its top-level list. */
void Task_Unlink(Task* task) {
  Task* parent;
  Task** list;

  if (task->prev != NULL) {
    task->prev->next = task->next;
  }
  if (task->next != NULL) {
    task->next->prev = task->prev;
  }

  parent = task->parent;
  if ((int)parent > TASK_LEVELS) {
    if (parent->firstChild == task) {
      parent->firstChild = task->next;
    }
  } else {
    /* a top-level task keeps its list level in parent */
    list = &data_027c027c->taskLists[(int)parent];
    if (*list == task) {
      *list = task->next;
    }
  }
  task->prev = NULL;
  task->next = NULL;
}
