#include "types.h"
#include "game/Unk_02080440.h"
#include "game/task.h"

extern Unk_02080440* data_027c027c;

/* Puts task at the head of top-level list number level. */
void Task_AddToList(Task* task, int level) {
  Task** list;

  list = &data_027c027c->taskLists[level];
  if (*list != NULL) {
    (*list)->prev = task;
    task->next = *list;
  }
  /* a top-level task keeps its list level in parent */
  task->parent = (Task*)level;
  *list = task;
}
