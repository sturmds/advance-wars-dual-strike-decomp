#ifndef GAME_OBJECT_ATTACK_TASK_H
#define GAME_OBJECT_ATTACK_TASK_H

#include "types.h"
#include "game/map_object.h"
#include "game/map_unit.h"

/* Work area of the task (script data_02168380) that shows the acting unit
 * attacking a map object or a tile with hp. */
typedef struct ObjectAttackTask {
  u8 pad_00[0x2c];
  int tileX; /* set for a tile attack only */
  int tileY;
  u8 pad_34[0x4c - 0x34];
  MapObject* object; /* NULL: a tile attack */
  u8 pad_50[0x54 - 0x50];
  MapUnit* attacker;
  u8 pad_58[0x64 - 0x58];
  u16 flag; /* the attack functions' flag argument */
} ObjectAttackTask;

#endif
