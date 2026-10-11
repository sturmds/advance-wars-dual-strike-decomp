#include "types.h"
#include "game/task.h"

BOOL func_02038064(void);

extern TaskScriptCmd data_02159ca8[];

void func_02032410(void)
{
    Task_Create(data_02159ca8, TASK_LIST(3));
    Task_Create(data_02159ca8, TASK_LIST(3));
    if (!func_02038064()) {
        return;
    }
    Task_Create(data_02159ca8, TASK_LIST(3));
    Task_Create(data_02159ca8, TASK_LIST(3));
}
