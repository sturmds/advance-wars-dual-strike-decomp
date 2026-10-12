#ifndef GAME_REPAIR_TASK_H
#define GAME_REPAIR_TASK_H

#include "types.h"
#include "game/map_unit.h"

/* Work area of the repair task (script data_02168538). */
typedef struct RepairTask {
  u8 pad_00[0x2c];
  int tileX;
  int tileY;
  u8 pad_34[0x4c - 0x34];
  MapUnit* unit;
} RepairTask;

#endif
