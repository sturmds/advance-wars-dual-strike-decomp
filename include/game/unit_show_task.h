#ifndef GAME_UNIT_SHOW_TASK_H
#define GAME_UNIT_SHOW_TASK_H

#include "types.h"
#include "game/map_unit.h"

/* Task made by UnitShowTask_Create for a unit the AI is showing. The first
 * 0x30 bytes belong to the task system (Task_Create). */
typedef struct UnitShowTask {
    u8 pad_00[0x30];
    MapUnit *unit;                     /* flags have SELECTED and LOCKED set while the task lives */
} UnitShowTask;

#endif
