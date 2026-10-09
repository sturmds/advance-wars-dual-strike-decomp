#include "types.h"
#include "game/task.h"

void func_02023760(Task *task);

/* Callback that func_020235ec hands to func_02023674 to resume the script
 * of every task running a given script. */
void Task_ResumeScriptVisitor(Task *task)
{
    func_02023760(task);
}
