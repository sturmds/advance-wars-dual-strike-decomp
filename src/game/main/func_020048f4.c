#include "types.h"
#include "game/task.h"

extern TaskScriptCmd data_02157284[];
extern TaskScriptCmd data_02157420[];

int func_02004a14(void);
Task *Task_Create(const void *script, int level);

void func_020048f4(u16 value) {
    Task *task;

    if (func_02004a14() == 0) {
        task = Task_Create(data_02157420, 3);
    } else {
        task = Task_Create(data_02157284, 3);
    }
    if (task != NULL) {
        task->unk_64 = value;
    }
}
