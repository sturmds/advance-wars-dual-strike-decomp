#include "types.h"
#include "game/unit_show_task.h"

void Task_Destroy(UnitShowTask* task);
void func_020b98bc(void);

/* Frees the unit (clears SELECTED and LOCKED) and destroys the task. */
void UnitShowTask_Destroy(UnitShowTask* task) {
  MapUnit* unit = task->unit;
  if (unit != NULL) {
    unit->flags &= ~(MAP_UNIT_SELECTED | MAP_UNIT_LOCKED);
  }
  Task_Destroy(task);
  func_020b98bc();
}
