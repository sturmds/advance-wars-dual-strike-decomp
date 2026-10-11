#include "types.h"
#include "game/task.h"

int func_0207b4c8(void);
void Task_ResumeScript(Task *task);

/* Resumes the script unless func_0207b4c8 returns 1. */
void LinkMenu_ResumeScript(Task *task)
{
    if (func_0207b4c8() == 1) {
        return;
    }
    Task_ResumeScript(task);
}
