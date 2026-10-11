#include "types.h"
#include "game/task.h"

extern const TaskScriptCmd data_02160ed0[];
extern const TaskScriptCmd data_02160f00[];
extern const TaskScriptCmd data_02160f30[];
extern const TaskScriptCmd data_02160f60[];

/* Starts the script task of team row 1 to 4. */
void func_020886cc(int row)
{
    switch (row) {
    case 1:
        Task_Create(data_02160ed0, TASK_LIST(3));
        break;
    case 2:
        Task_Create(data_02160f00, TASK_LIST(3));
        break;
    case 3:
        Task_Create(data_02160f30, TASK_LIST(3));
        break;
    case 4:
        Task_Create(data_02160f60, TASK_LIST(3));
        break;
    }
}
