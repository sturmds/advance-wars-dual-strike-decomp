#include "types.h"
#include "game/task.h"

extern u32 gLinkPlay;
extern const TaskScriptCmd data_021604e4[];

/* Starts the data_021604e4 task (func_02080708 destroys it) while gLinkPlay is set. */
void func_02080734(void) {
  if (gLinkPlay == 0) {
    return;
  }
  Task_Create(data_021604e4, TASK_LIST(3));
}
