#include "types.h"
#include "game/task.h"

void func_02023620(Task *task);
void func_02023674(void *script, void (*visit)(Task *task));

/* Destroys every task that was started with this script (Task.unk_00). */
void Task_DestroyByScript(void *script)
{
    func_02023674(script, func_02023620);
}
