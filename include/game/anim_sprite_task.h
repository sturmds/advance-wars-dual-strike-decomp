#ifndef GAME_ANIM_SPRITE_TASK_H
#define GAME_ANIM_SPRITE_TASK_H

#include "types.h"

/* Task made by Sprite_CreateTask (script data_02159114) and set up by
 * TableSpriteTask_Init. It plays a halfword command list and draws one frame of
 * a frame table as a textured quad. The first 0x2c bytes belong to the
 * task system (Task_Create). */
typedef struct AnimSpriteTask {
  u8 pad_00[0x2c];
  void (*callback)(
      struct AnimSpriteTask*
          task); /* run by ANIM_SPRITE_CMD_CALLBACK, and at the end */
  const void* const* frames; /* frame table: first word of the sprite data */
  const u16* frame;          /* frame drawn now, from frames */
  const u16* start;          /* first command of the animation */
  const u16* cmd;            /* command that runs next */
  const s16* scrollX;        /* subtracted from x when set */
  const s16* scrollY;        /* subtracted from y when set */
  const u8* paused; /* the animation holds while this is set and nonzero */
  s16 x;
  s16 y;
  s16 unk_50;
  u16 timer; /* frames the current command has shown */
  u16 tile;
  s16 scaleX; /* 256 = 1.0 */
  s16 scaleY;
  u8 unk_5a;
  u8 unk_5b;      /* 0xff after set-up; ANIM_SPRITE_CMD_SET_5B writes it */
  u8 held : 1;    /* stopped on ANIM_SPRITE_CMD_HOLD */
  u8 playing : 1; /* cleared on ANIM_SPRITE_CMD_END */
  u8 cull : 1; /* guess: AnimSpriteTask_Draw skips the quad when off screen */
  u8 callbackRan : 1; /* skips the callback at the end */
} AnimSpriteTask;

/* Commands: a halfword (frame << 8 | updates to show it - 1), or one of these in
 * the high byte. */
#define ANIM_SPRITE_CMD_MASK 0xff00
#define ANIM_SPRITE_CMD_END \
  0xff00 /* run the callback and resume the task script */
#define ANIM_SPRITE_CMD_HOLD 0xfe00 /* stay on the current frame */
#define ANIM_SPRITE_CMD_LOOP 0xfd00 /* back to the first command */
#define ANIM_SPRITE_CMD_CALLBACK 0xfc00
#define ANIM_SPRITE_CMD_SET_5B 0xfb00 /* low byte to unk_5b */

#endif
