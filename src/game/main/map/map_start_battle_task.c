#include "types.h"

void *Task_Create(const void *script, void *parentOrList);

extern const u32 data_02166d98[];

/* Starts the data_02166d98 task on top-level task list 3. */
void Map_StartBattleTask(void)
{
    Task_Create(data_02166d98, (void *)3);
}
