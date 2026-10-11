#ifndef GAME_SPRITE_ANIM_H
#define GAME_SPRITE_ANIM_H

#include "types.h"
#include "sdk/fx.h"

#define SPRITE_ANIM_COUNT 16

/* SpriteAnim.flags bits */
#define SPRITE_ANIM_TILES_PENDING 0x01 /* tiles still to be copied to VRAM */
#define SPRITE_ANIM_HALFWORD_FILE 0x02 /* file stores halfword offsets, not word pointers */
#define SPRITE_ANIM_TILE_DIRECT   0x04 /* tiles copied right away, not through the transfer queue */
#define SPRITE_ANIM_DRAW_FLAG     0x08 /* passed on as the last argument of SpriteCells_Draw3d */
#define SPRITE_ANIM_ALT_REGISTER  0x10 /* draw with func_020224ec instead of func_020224d4 */
#define SPRITE_ANIM_SKIP_DRAW     0x20 /* skip the next draw, then clears itself */

/* One of the SPRITE_ANIM_COUNT animated sprites in gSpriteAnims, 0x30 bytes
 * each. A sprite is free while data is 0. It plays an animation from a loaded
 * sprite file. Task scripts hold pointers to these. */
typedef struct SpriteAnim {
    const void *data;                  /* the loaded file; 0 for a free slot */
    const void *unk_04;                /* the file's first section */
    const void *animation;             /* current animation, from the file's animation table */
    const void *step;                  /* current SpriteAnimStep; starts at animation */
    const void *cells;                 /* OAM cells of the current frame (Sprite_Queue's data) */
    const void *affine;                /* affine parameter sets of the current frame */
    s16 framesLeft;                    /* frames left in the current step */
    u16 speed;                         /* 0x100: one frame per tick, 0: paused */
    u16 speedFraction;                 /* speed-scaled frames carried to the next step, below 0x100 */
    u16 layer;                         /* sprite list for Sprite_Queue */
    u8 flags;                          /* SPRITE_ANIM_* */
    u8 affineBase;                     /* first affine slot of the sprite, shifted to bit 9 of the x word */
    u16 lastCommand;                   /* value of the last unknown command step */
    u32 tileBase;                      /* Sprite_Queue's unk08 */
    u32 unk_28;
    void *tileSource;                  /* tiles copied to VRAM while SPRITE_ANIM_TILES_PENDING is set */
} SpriteAnim;

/* One entry of a frame's affine data (SpriteAnim.affine: a u16 count in bits 0-14, then
 * the entries). The matrix rotates by angle (low byte, 256 steps) and divides by the scales;
 * 0x100 = 1x (sine table in 4.12, OAM parameters in 8.8). */
typedef struct SpriteAffineEntry {
    u16 angle;
    u16 scaleX;
    u16 scaleY;
} SpriteAffineEntry;

/* Task that draws a SpriteAnim at a screen position every frame. The first
 * 0x50 bytes belong to the task system (Task_Create). */
typedef struct SpriteAnimTask {
    u8 pad_00[0x50];
    SpriteAnim *sprite;
    int x;
    int y;
} SpriteAnimTask;

/* Starts a SpriteAnimTask that draws animation of file at (x, y). */
int SpriteAnim_Draw(SpriteAnim *sprite, int x, int y);
SpriteAnimTask *SpriteAnimTask_Start(const void *file, int x, int y, u32 tileBase, int animation, int layer);

/* Same for a sprite drawn in 3D space. */
typedef struct SpriteAnim3dTask {
    u8 pad_00[0x50];
    SpriteAnim *sprite;
    VecFx32 pos;
} SpriteAnim3dTask;

/* One step of an animation script: a list of (duration, value) halfword
 * pairs. A pair with a duration shows cell set `value` for that many
 * frames, scaled by speed / 0x100. A pair with duration 0 is a command. */
typedef struct SpriteAnimStep {
    u16 duration;
    u16 value;
} SpriteAnimStep;

#define SPRITE_ANIM_CMD_STOP    0    /* hold the last frame */
#define SPRITE_ANIM_CMD_FREE    1    /* free the sprite (SpriteAnim_Free) */
#define SPRITE_ANIM_CMD_LOOP    0xff /* jump back to the first step */
#define SPRITE_ANIM_AFFINE_CELLS 0x8000 /* cell set starts with affine parameter sets */

extern SpriteAnim gSpriteAnims[SPRITE_ANIM_COUNT];

#endif
