#include "types.h"

void *func_02023a0c(void *taskDesc, int priority);

extern u8 data_021687e0[];

/* Shared tail of the CO Powers that play a sound and then start one
 * common task (HachiFlash, SonjaFlash, WarBonds, GoldRush, MarketCrash).
 * data_021687e0 describes that task. priority is a guess: func_02023a58
 * picks a task list by whether it is below 8. */
void func_020e2670(int priority)
{
    func_02023a0c(data_021687e0, priority);
}
