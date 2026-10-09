#include "types.h"
#include "game/task.h"

void Task_ResumeScript(Task *task);

/* Callback that func_020235ec hands to Task_ForEachByScript to resume the script
 * of every task running a given script. */
void Task_ResumeScriptVisitor(Task *task)
{
    Task_ResumeScript(task);
}
