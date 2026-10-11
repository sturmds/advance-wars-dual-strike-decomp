#include "types.h"
#include "game/task.h"

BOOL func_0207a1dc(void);
void Task_ResumeScript(Task *task);
void Link_Stop(void);

void func_0207ab2c(Task *task)
{
    if (func_0207a1dc() != FALSE) {
        return;
    }
    Link_Stop();
    Task_ResumeScript(task);
}
