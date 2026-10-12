#ifndef GAME_MISSILE_STRIKE_TASK_H
#define GAME_MISSILE_STRIKE_TASK_H

#include "types.h"

/* Work area of the missile strike task (script data_02169088), started on
 * list 3 by MissileStrike_Start. The first 0x34 bytes belong to the task system. */
typedef struct MissileStrikeTask {
  u8 pad_00[0x34];
  int tileX; /* tile the strike lands on */
  int tileY;
  int unk_3c; /* set to 8 */
  int pixelX; /* where the missile lands on the map, in pixels (read by MissileStrike_StartBlastAnim and MissileStrike_FallStep) */
  int pixelY;
  int unk_48;
  u8 pad_4c[0x54 - 0x4c];
  int fallHeight; /* pixels left to fall: MissileStrike_Start stores its third argument, MissileStrike_Init sets pixelY */
  int unk_58;     /* fourth argument (80 from func_020f1f38) */
} MissileStrikeTask;

#endif
