#include "types.h"
#include "game/task.h"

extern const TaskScriptCmd gRowLockMarkRow1Script[];
extern const TaskScriptCmd gRowLockMarkRow2Script[];
extern const TaskScriptCmd gRowLockMarkRow3Script[];
extern const TaskScriptCmd gRowLockMarkRow4Script[];

/* Starts the script task of team row 1 to 4. */
void RowLockMark_Start(int row) {
  switch (row) {
    case 1:
      Task_Create(gRowLockMarkRow1Script, TASK_LIST(3));
      break;
    case 2:
      Task_Create(gRowLockMarkRow2Script, TASK_LIST(3));
      break;
    case 3:
      Task_Create(gRowLockMarkRow3Script, TASK_LIST(3));
      break;
    case 4:
      Task_Create(gRowLockMarkRow4Script, TASK_LIST(3));
      break;
  }
}
