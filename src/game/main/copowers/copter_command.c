#include "types.h"
#include "game/task.h"
#include "game/UnitDropTask.h"
#include "game/unit_stats.h"

/* Script of (opcode, argument) pairs, shared by Sensei's two powers. */
extern const TaskScriptCmd gUnitDropScript[];

/* Sensei's CO Power: places infantry units on the map. */
void CopterCommand(void* parent) {
  UnitDropTask* task = (UnitDropTask*)Task_CreateChild(gUnitDropScript, parent);
  task->unitType = UNIT_INFANTRY;
}
