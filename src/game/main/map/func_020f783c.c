#include "types.h"
#include "game/Unk_02080440.h"
#include "game/task.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define HOOKS (data_027c027c->unk_50[data_027c0284])

/* Starts the task the screen config names in unk_60, if any, and copies the
 * calling task's unk_64 to it. */
void func_020f783c(Task* parent) {
  Task* task;

  if (HOOKS->unk_60 != NULL) {
    task = Task_Create(HOOKS->unk_60, parent);
    /* needed to match: the original reads the halfword signed */
    task->unk_64 = *(s16*)&parent->unk_64;
  }
}
