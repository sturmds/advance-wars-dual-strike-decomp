#include "types.h"
#include "game/task.h"

extern const TaskScriptCmd data_021687e0[];

/* Shared tail of the CO Powers that play a sound and then start one
 * common task (HachiFlash, SonjaFlash, WarBonds, GoldRush, MarketCrash).
 * data_021687e0 describes that task. */
void func_020e2670(Task* parent) { Task_CreateChild(data_021687e0, parent); }
