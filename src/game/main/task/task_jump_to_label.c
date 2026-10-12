#include "types.h"
#include "game/task.h"

/* Moves the task's script to the label command with this id and clears
 * update so the script continues from there. Does nothing if the script
 * has no such label. */
void Task_JumpToLabel(Task* task, int label) {
  const TaskScriptCmd* cmd;

  for (cmd = task->scriptStart; cmd->op != TASK_CMD_END; cmd++) {
    if (cmd->op == TASK_CMD_LABEL && cmd->arg == label) {
      task->pc = cmd;
      task->update = NULL;
      return;
    }
  }
}
