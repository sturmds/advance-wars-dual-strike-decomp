#ifndef GAME_TILE_EFFECT_TASK_H
#define GAME_TILE_EFFECT_TASK_H

#include "types.h"

/* Task TileEffectTask_Create makes to play an effect on a map tile. The first
 * 0x2c bytes belong to the task system (Task_Create). */
typedef struct TileEffectTask {
  u8 pad_00[0x2c];
  int x; /* tile */
  int y;
  u8 pad_34[0x4a - 0x34];
  u16 unk_4a; /* passed on as the tile of SiloMissileFall_Start */
  u8 pad_4c[0x50 - 0x4c];
  void* sprite; /* NULL until made; TileEffectTask_Destroy frees it */
  int effect;   /* effect id, 0-35 (MapSpriteTask_PlayKindEffects) */
} TileEffectTask;

#endif
