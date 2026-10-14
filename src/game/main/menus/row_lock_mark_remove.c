#include "types.h"
#include "game/task.h"

void Task_ResumeByScript(void* script);
extern TaskScriptCmd gRowLockMarkRow1Script[];
extern TaskScriptCmd gRowLockMarkRow2Script[];
extern TaskScriptCmd gRowLockMarkRow3Script[];
extern TaskScriptCmd gRowLockMarkRow4Script[];

/* Resumes the task that runs the script of team row 1 to 4. */
void RowLockMark_Remove(int row) {
  switch (row) {
    case 1:
      Task_ResumeByScript(gRowLockMarkRow1Script);
      break;
    case 2:
      Task_ResumeByScript(gRowLockMarkRow2Script);
      break;
    case 3:
      Task_ResumeByScript(gRowLockMarkRow3Script);
      break;
    case 4:
      Task_ResumeByScript(gRowLockMarkRow4Script);
      break;
  }
}
