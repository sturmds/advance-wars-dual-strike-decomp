#include "types.h"
#include "game/task.h"

void Task_Destroy(Task* task);

/* Callback that Task_DestroyByScript hands to Task_ForEachByScript. */
void Task_DestroyVisitor(Task* task) { Task_Destroy(task); }
