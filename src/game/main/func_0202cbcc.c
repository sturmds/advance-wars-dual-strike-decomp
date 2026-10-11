#include "types.h"
#include "game/task.h"

int Link_IsActive(void);
extern const TaskScriptCmd data_02159988[];

void func_0202cbcc(Task *parent)
{
    if (Link_IsActive() == 1) {
        Task_CreateChild(data_02159988, parent);
    }
}
