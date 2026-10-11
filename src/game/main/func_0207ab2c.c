#include "types.h"
#include "game/task.h"

BOOL Wireless_GetChildAids(void);
void Task_ResumeScript(Task *task);
void Link_Stop(void);

void func_0207ab2c(Task *task)
{
    if (Wireless_GetChildAids() != FALSE) {
        return;
    }
    Link_Stop();
    Task_ResumeScript(task);
}
