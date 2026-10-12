#ifndef GAME_NUMBER_SPRITE_TASK_H
#define GAME_NUMBER_SPRITE_TASK_H

#include "types.h"

/* Task of the gNumberSpriteTaskScript script (NumberSpriteTask_Start makes it): draws a number
 * with NumberSprite_Put* on the second screen every frame. The first 0x2c
 * bytes belong to the task system. */
typedef struct NumberSpriteTask {
  u8 pad_00[0x2c];
  int x;
  int y;
  u8 pad_34[0x54 - 0x34];
  int slideOffset; /* starts at -32, moves to 0 two pixels a frame; added to y */
  int value;       /* number to draw */
  u8 pad_5c[0x64 - 0x5c];
  s16 firstTile; /* first object tile of the number font */
  s16 palette;
  s16 style; /* 0: hours and minutes, 1: label after the number, 2: label before it */
} NumberSpriteTask;

#endif
