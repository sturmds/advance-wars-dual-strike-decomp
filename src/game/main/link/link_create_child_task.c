#include "types.h"
#include "game/task.h"

extern const TaskScriptCmd data_0215f8ec[];

int Link_IsActive(void); /* the link mode flag, 1 in some link mode */

/* Starts the link task script data_0215f8ec as a child of parent, only when Link_IsActive is not 0. */
void Link_CreateChildTask(Task* parent) {
  if (Link_IsActive() == 0) {
    return;
  }
  Task_CreateChild(data_0215f8ec, parent);
}
