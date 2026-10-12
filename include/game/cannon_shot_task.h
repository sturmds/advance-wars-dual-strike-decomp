#ifndef GAME_CANNON_SHOT_TASK_H
#define GAME_CANNON_SHOT_TASK_H

#include "types.h"

/* Task of script data_02168088 that shows a Black Cannon or Minicannon
 * shot (CannonShotTask_Start starts it): the muzzle flash on the cannon, then the
 * impact on the target tile. The first 0x54 bytes belong to the task
 * system. */
typedef struct CannonShotTask {
  u8 pad_00[0x54];
  int cannonX; /* tile of the cannon object */
  int cannonY;
  int unk_5c; /* set to 0 */
  u8 pad_60[0x64 - 0x60];
  s16 targetX; /* tile of the unit hit */
  s16 targetY;
  s16 facing;     /* MapObject.variant of the cannon */
  s16 cannonKind; /* MAP_OBJECT_BLACK_CANNON or MAP_OBJECT_MINICANNON */
} CannonShotTask;

#endif
