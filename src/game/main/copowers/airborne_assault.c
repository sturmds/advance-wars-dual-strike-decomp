#include "types.h"
#include "game/UnitDropTask.h"
#include "game/unit_stats.h"

/* Starts a task that runs `script` as a child of `parent`; returns the task. */
UnitDropTask *Task_CreateChild(const void *script, void *parent);

/* Script of (opcode, argument) pairs, shared by Sensei's two powers. */
extern const u32 gUnitDropScript[];

/* Sensei's Super CO Power: places mech units on the map. */
void AirborneAssault(void *parent) {
    UnitDropTask *task = Task_CreateChild(gUnitDropScript, parent);
    task->unitType = UNIT_MECH;
}
