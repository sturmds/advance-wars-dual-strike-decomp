#include "types.h"
#include "game/task.h"

int Link_IsActive(void);
extern const TaskScriptCmd data_021673a4[];

void func_020d6a34(Task *parent)
{
    if (Link_IsActive() == 1) {
        Task_CreateChild(data_021673a4, parent);
    }
}
