#include "types.h"
#include "game/task.h"

/* With update cleared, the script interpreter (func_020227e8) runs the
 * task's script again from its current command. */
void Task_ResumeScript(Task *task)
{
    task->update = NULL;
}
