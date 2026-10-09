#include "types.h"
#include "game/task.h"

extern u32 data_0229eaec;
extern const TaskScriptCmd data_021604e4[];

/* Starts the data_021604e4 task (func_02080708 destroys it) while data_0229eaec is set. */
void func_02080734(void)
{
    if (data_0229eaec == 0) {
        return;
    }
    Task_Create(data_021604e4, TASK_LIST(3));
}
