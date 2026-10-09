#include "types.h"

/* Task started for an activated power. The first 0x54 bytes belong to the
 * task system (Task_Create). */
typedef struct PowerActivateTask {
    u8 pad_00[0x54];
    int player;
    int power;
} PowerActivateTask;

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
