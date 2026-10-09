#include "types.h"
#include "game/task.h"

BOOL Text_IsPrinting(void);

void func_0201c748(Task *task)
{
    if (Text_IsPrinting() == FALSE) {
        task->pc = NULL;
    }
}
