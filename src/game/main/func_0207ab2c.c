#include "types.h"
#include "game/task.h"

BOOL func_0207a1dc(void);
void Task_ResumeScript(Task *task);
void func_0207baf4(void);

void func_0207ab2c(Task *task)
{
    if (func_0207a1dc() != FALSE) {
        return;
    }
    func_0207baf4();
    Task_ResumeScript(task);
}
