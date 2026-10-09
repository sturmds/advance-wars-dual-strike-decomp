#include "types.h"
#include "game/task.h"

BOOL func_02014f58(void);

void func_0201c748(Task *task)
{
    if (func_02014f58() == FALSE) {
        task->pc = NULL;
    }
}
