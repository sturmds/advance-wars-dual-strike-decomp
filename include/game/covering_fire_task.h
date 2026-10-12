#ifndef GAME_COVERING_FIRE_TASK_H
#define GAME_COVERING_FIRE_TASK_H

#include "types.h"

/* Task that runs gCoveringFireScript (Rachel's Super CO Power). The first
 * 0x2c bytes belong to the task system (Task_CreateChild). */
typedef struct CoveringFireTask {
  u8 pad_00[0x2c];
  int targetX; /* tile of the missile being fired */
  int targetY;
} CoveringFireTask;

#endif
