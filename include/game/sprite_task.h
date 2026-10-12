#ifndef GAME_SPRITE_TASK_H
#define GAME_SPRITE_TASK_H

#include "types.h"

/* Task made from gSpriteTaskScript by SpriteTask_Create. The first 0x2c bytes belong
 * to the task system (Task_Create). */
typedef struct SpriteTask {
  u8 pad_00[0x2c];
  int x;
  int y;
  u16 tile; /* first tile of its picture in object VRAM */
  u16 paletteSlot;
} SpriteTask;

#endif
