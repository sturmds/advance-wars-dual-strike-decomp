#ifndef GAME_FALLING_SPRITES_H
#define GAME_FALLING_SPRITES_H

#include "types.h"

#define FALLING_SPRITE_COUNT 16

/* One sprite that falls across both screens: y 57-255 shows on the
 * screen of data_027c0288.unk_04 at y - 56, y 249-447 on the screen of
 * unk_00 at y - 256. */
typedef struct FallingSprite {
  u32 x : 9;
  u32 y : 9;
  u32 timer : 9; /* frames shown of the current animation frame */
  u32 frame : 4; /* animation frame, index into its FallingSpriteFrame table */
  u32 fast
      : 1; /* 1: second table (data_021599f8), falls 2 pixels every other frame */
} FallingSprite;

/* One animation frame: the sprite cell to draw and for how many frames.
 * A duration of 0 ends the table. */
typedef struct FallingSpriteFrame {
  u8 cell;
  u8 duration;
} FallingSpriteFrame;

/* The task (script data_02159a0c) that runs the sprites keeps them in its
 * own Task block, after the fields every task uses. */
typedef struct FallingSpriteTask {
  u8 pad_00[0x2c]; /* Task fields up to waitFrames */
  FallingSprite sprites[FALLING_SPRITE_COUNT];
} FallingSpriteTask;

/* One sprite of a shower (script data_02159ca8) that falls once across
 * both screens: y 313-511 shows on the screen of data_027c0288.unk_04 at
 * y - 312, y 505-703 on the screen of unk_00 at y - 512; x shows at
 * x - 512. Uses the FallingSpriteFrame tables data_02159c64 (variant 0)
 * and data_02159c50 (variant 1). */
typedef struct ShowerSprite {
  u32 x : 10;
  u32 y : 10;
  u32 timer : 6; /* frames shown of the current animation frame */
  u32 frame : 4;
  u32 variant : 1; /* 1: falls 2 pixels every other frame */
  u32 active : 1;  /* cleared once the sprite leaves both screens */
} ShowerSprite;

typedef struct ShowerSpriteTask {
  u8 pad_00[0x2c]; /* Task fields up to waitFrames */
  ShowerSprite sprites[FALLING_SPRITE_COUNT];
} ShowerSpriteTask;

#endif
