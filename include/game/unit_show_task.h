#ifndef GAME_UNIT_SHOW_TASK_H
#define GAME_UNIT_SHOW_TASK_H

#include "types.h"
#include "game/map_unit.h"

/* Task made by UnitShowTask_Create for a unit the AI is showing. The first
 * 0x30 bytes belong to the task system (Task_Create). */
typedef struct UnitShowTask {
    u8 pad_00[0x2c];
    void *sprite;                      /* 2D or 3D sprite handle */
    MapUnit *unit;                     /* flags have SELECTED and LOCKED set while the task lives */
    u8 pad_34;
    u8 unk_35;
    u8 pad_36[0x40 - 0x36];
    s16 x;                             /* pixels: tile * 16 */
    s16 y;
} UnitShowTask;

#endif
