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
    u32 timer : 9;                     /* frames shown of the current animation frame */
    u32 frame : 4;                     /* animation frame, index into its FallingSpriteFrame table */
    u32 fast : 1;                      /* 1: second table (data_021599f8), falls 2 pixels every other frame */
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
    u8 pad_00[0x2c];                   /* Task fields up to waitFrames */
    FallingSprite sprites[FALLING_SPRITE_COUNT];
} FallingSpriteTask;

#endif
