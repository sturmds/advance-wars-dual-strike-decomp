#ifndef GAME_QUAD_SPRITE_TASK_H
#define GAME_QUAD_SPRITE_TASK_H

#include "types.h"

/* One textured quad of a QuadSpriteTask frame, 8 bytes. A frame is a list
 * of these, ended by one with width 0. */
typedef struct QuadSpritePart {
  u16 x : 9; /* signed, from the sprite's origin */
  u16 width : 7;
  u16 y : 9; /* signed */
  u16 height : 7;
  u16 tile;     /* added to QuadSpriteTask.tileBase */
  u8 sizeS : 4; /* texture size codes (8 << code pixels) */
  u8 sizeT : 4;
  u8 flipY : 1;
  u8 flipX : 1;
  u8 palette : 4; /* added to QuadSpriteTask.paletteBase */
  u8 z : 2;       /* added to QuadSpriteTask.z */
} QuadSpritePart;

/* Task made by Sprite_CreateTask (scripts data_02159114 and data_02159134)
 * that draws its current frame as 3D quads. The first 0x2c bytes belong
 * to the task system (Task_Create). */
typedef struct QuadSpriteTask {
  u8 pad_00[0x34];
  const QuadSpritePart* frame; /* quads drawn this frame, or NULL */
  u8 pad_38[0x40 - 0x38];
  const s16* scrollX; /* subtracted from x when set */
  const s16* scrollY;
  u8 pad_48[0x4c - 0x48];
  s16 x;
  s16 y;
  s16 z;
  u8 pad_52[0x54 - 0x52];
  u16 tileBase;
  s16 scaleX; /* 256 = 1.0 */
  s16 scaleY;
  u8 paletteBase;
  u8 pad_5b;
  u8 unk_5c_0 : 1; /* cleared by TableSpriteTask_Init */
  u8 unk_5c_1 : 1; /* set by TableSpriteTask_Init */
  u8 cull : 1; /* skip quads that lie off screen; Sprite_CreateTask sets it */
  u8 unk_5c_3 : 1; /* cleared by TableSpriteTask_Init */
} QuadSpriteTask;

#endif
