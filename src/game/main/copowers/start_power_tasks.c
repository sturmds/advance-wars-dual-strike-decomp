#include "types.h"
#include "game/power_activate_task.h"

PowerActivateTask *Task_Create(const void *script, void *parentOrList);
PowerActivateTask *Task_CreateChild(const void *script, void *parent);

extern const u32 data_02166f24[];
extern const u32 data_02166f5c[];

void Power_StartTasks(int player, int power, BOOL startSecond)
{
    PowerActivateTask *task = Task_Create(data_02166f5c, (void *)3);
    PowerActivateTask *child;

    task->player = player;
    task->power = power;
    if (startSecond) {
        child = Task_CreateChild(data_02166f24, task);
        child->player = player;
        child->power = power;
    }
}
