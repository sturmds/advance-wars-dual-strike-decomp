#ifndef GAME_POWER_HIT_EFFECT_TASK_H
#define GAME_POWER_HIT_EFFECT_TASK_H

#include "types.h"

/* Task that plays the effect of a power on one tile (started by
 * PowerHitEffect_Start). The first 0x2c bytes belong to the task system
 * (Task_Create). */
typedef struct PowerHitEffectTask {
  u8 pad_00[0x2c];
  u8 player; /* whose front CO picks the effect and sound */
  u8 pad_2d[0x40 - 0x2d];
  int tileX;
  int tileY;
} PowerHitEffectTask;

#endif
