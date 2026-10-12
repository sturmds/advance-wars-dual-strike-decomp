#include "types.h"
#include "game/map_object_turn_task.h"
#include "game/task.h"

extern int data_022a6dd4;

void Task_Destroy(Task* task);

/* Step of the map object fire script: ends the task once every entry of the
 * AI target list has been shown. */
void MapObjectFireTask_FinishWhenDone(MapObjectFireTask* task) {
  if (task->nextEntry != data_022a6dd4) {
    return;
  }
  Task_Destroy((Task*)task);
}
