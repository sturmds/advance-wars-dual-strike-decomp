#ifndef GAME_ZOOM_SPRITE_TASK_H
#define GAME_ZOOM_SPRITE_TASK_H

#include "types.h"
#include "sdk/fx.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/sprite_anim.h"

/* Task of ZoomSpriteTask_Init and ZoomSpriteTask_InitWithSound: a sprite with a graphic for the
 * game language that zooms from scale 0x200 down to 0x100 at a map
 * position. The first 0x2c bytes belong to the task system (Task_Create). */
typedef struct ZoomSpriteTask {
  u8 pad_00[0x2c];
  int x; /* map position in pixels */
  int y;
  u8 pad_34[0x44 - 0x34];
  s16 holdFrames; /* -1 until the zoom ends, then frames left to wait */
  u8 pad_46[0x50 - 0x46];
  SpriteAnim* sprite;
  int animation; /* the next one is used on the right half of the screen */
  u8 pad_58[0x64 - 0x58];
  s16 scale; /* 0x100 = 1x */
} ZoomSpriteTask;

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern Unk_027c0288 data_027c0288;
/* Sine by 1/256 of a turn, 4096 = 1.0. The last 64 entries repeat the first
   64, so that gSineTable[i + 64] gives the cosine of i for i up to 255. */
extern const s16 gSineTable[];

s32 HwDiv32(s32 numerator, s32 denominator);
void Oam_SetAffineParams(int index, s16 pa, s16 pb, s16 pc, s16 pd);
int SpriteAnim_Draw(SpriteAnim* sprite, int x, int y);
int SpriteAnim_Draw3d(SpriteAnim* sprite, VecFx32* pos, VecFx32* scale);

#define ZOOM_DIVISOR(scale) ((scale) ? (scale) : 2)

/* Draws the sprite at task->scale: in 3D on the screen whose state is
 * data_027c0288.unk_00, otherwise as a 2D sprite with affine matrix 0,
 * against the map scroll. */
static inline void ZoomSpriteTask_Draw(ZoomSpriteTask* task) {
  VecFx32 pos;
  VecFx32 scale;

  if (data_027c027c == data_027c0288.unk_00) {
    scale.x = scale.y = HwDiv32(task->scale << 12, 232);
    scale.z = 0x1000;
    pos.x = (task->x << 13) / 256;
    pos.y = task->y * 0x6000 / 0x300;
    pos.z = 8;
    SpriteAnim_Draw3d(task->sprite, &pos, &scale);
  } else {
    s16 s = task->scale;
    int d3 = ZOOM_DIVISOR(s);
    int d2 = ZOOM_DIVISOR(s);
    int d1 = ZOOM_DIVISOR(s);
    int d0 = ZOOM_DIVISOR(s);
    int sinValue = gSineTable[0];
    int cosValue = gSineTable[64] << 4;

    Oam_SetAffineParams(0, HwDiv32(cosValue, d0), HwDiv32(-sinValue << 4, d1),
                        HwDiv32(sinValue << 4, d2), HwDiv32(cosValue, d3));
    /* 0x300 sets OAM attribute 0 bits 8-9: affine, double size */
    SpriteAnim_Draw(
        task->sprite,
        (task->x - data_027c027c->unk_48[data_027c0284]->unk_1c4->scrollX -
         32) &
            0x1ff,
        ((task->y - data_027c027c->unk_48[data_027c0284]->unk_1c4->scrollY -
          16) &
         0xff) |
            0x300);
  }
}

#endif
