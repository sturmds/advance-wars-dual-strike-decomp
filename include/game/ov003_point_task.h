#ifndef GAME_OV003_POINT_TASK_H
#define GAME_OV003_POINT_TASK_H

#include "types.h"

/* The task of data_ov003_02364a8c: something drawn at a screen position
 * for 16 frames. Its fields overlay the generic Task fields from 0x2c. */
typedef struct Ov003PointTask {
  u8 pad_00[0x2c];
  int x;
  int y;
  u8 pad_34[0x4c - 0x34];
  s16 frame; /* counts to 16, then the script goes on */
} Ov003PointTask;

#endif
