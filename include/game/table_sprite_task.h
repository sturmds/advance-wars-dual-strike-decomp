#ifndef GAME_TABLE_SPRITE_TASK_H
#define GAME_TABLE_SPRITE_TASK_H

#include "types.h"

/* Task made by Sprite_CreateTask (script data_02159114) and set up by
 * TableSpriteTask_Init from a table of words. The first 0x2c bytes belong to
 * the task system (Task_Create). */
typedef struct TableSpriteTask {
  u8 pad_00[0x2c];
  u32 unk_2c;
  u32 unk_30; /* first word of the table */
  u32 unk_34;
  u32 unk_38; /* table word picked by the index argument, plus 1 */
  u32 unk_3c; /* same word as unk_38 */
  u32 unk_40;
  u32 unk_44;
  u32 unk_48;
  u16 unk_4c;
  s16 unk_4e;
  s16 unk_50;
  u16 unk_52;
  u16 tile;   /* first tile of its picture in object VRAM */
  s16 scaleX; /* 0x100 is full size */
  s16 scaleY;
  u8 unk_5a;
  u8 unk_5b; /* 0xff on a new task */
  u8 flags; /* bit 0 cleared, bit 1 set, bits 2 and 3 cleared by TableSpriteTask_Init */
} TableSpriteTask;

#endif
