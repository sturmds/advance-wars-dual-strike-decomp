#include "types.h"
#include "game/task.h"

typedef BOOL (*TaskCheckFunc)(void);

void Task_JumpToLabel(Task* task, int label);

/* Script command: calls the check function in the command's value. When
 * there is none, or it returns 0, jumps to the label named by arg;
 * otherwise goes on with the next command. */
BOOL TaskCmd_JumpToLabelUnlessCheck(Task* task) {
  TaskCheckFunc check = (TaskCheckFunc)task->pc->value;

  if (check == NULL || check() == 0) {
    Task_JumpToLabel(task, task->pc->arg);
  } else {
    task->pc++;
  }
  return TRUE;
}
