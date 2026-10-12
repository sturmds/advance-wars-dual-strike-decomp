#include "types.h"
#include "game/task.h"
#include "game/UnitDropTask.h"
#include "game/unit_stats.h"

/* Script of (opcode, argument) pairs, shared by Sensei's two powers. */
extern const TaskScriptCmd gUnitDropScript[];

/* Sensei's Super CO Power: places mech units on the map. */
void AirborneAssault(void* parent) {
  UnitDropTask* task = (UnitDropTask*)Task_CreateChild(gUnitDropScript, parent);
  task->unitType = UNIT_MECH;
}
