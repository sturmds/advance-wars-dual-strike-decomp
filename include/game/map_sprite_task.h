#ifndef GAME_MAP_SPRITE_TASK_H
#define GAME_MAP_SPRITE_TASK_H

#include "types.h"
#include "game/task.h"
#include "game/sprite_anim.h"

/* A task that draws one sprite object on the map (TileEffectTask_Update,
 * SiloMissileFall_Update place it against the map view scroll). Up to 0x50 it has
 * the Task layout; from 0x50 its own fields, where Task has fields that
 * other tasks use differently. */
typedef struct MapSpriteTask {
  u8 pad_00[0x14];
  Task* parent; /* Task.parent */
  u8 pad_18[0x50 - 0x18];
  SpriteAnim* sprite;
  int kind; /* picks the sound and the two effect tasks in MapSpriteTask_PlayKindEffects */
} MapSpriteTask;

#endif
