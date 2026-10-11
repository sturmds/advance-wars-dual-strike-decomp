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

/* One of the SPRITE_ANIM_COUNT animated sprites in data_0229b080, 0x30 bytes
 * each. A sprite is free while data is 0. It plays an animation from a loaded
 * sprite file. Task scripts hold pointers to these. */
typedef struct SpriteAnim {
    const void *data;                  /* the loaded file; 0 for a free slot */
    const void *unk_04;                /* the file's first section */
    const void *animation;             /* current animation, from the file's animation table */
    const void *unk_0c;                /* set to the same animation as the field before */
    const void *cells;                 /* OAM cells of the current frame (Sprite_Queue's data) */
    const void *affine;                /* affine parameter sets of the current frame */
    u16 unk_18;
    u16 unk_1a;                        /* 0x100 on a new sprite, restored after a reset */
    u16 unk_1c;
    u16 layer;                         /* sprite list for Sprite_Queue */
    u8 flags;                          /* SPRITE_ANIM_* */
    u8 affineBase;                     /* first affine slot of the sprite, shifted to bit 9 of the x word */
    u16 unk_22;
    u32 tileBase;                      /* Sprite_Queue's unk08 */
    u32 unk_28;
    void *tileSource;                  /* tiles copied to VRAM while SPRITE_ANIM_TILES_PENDING is set */
} SpriteAnim;

/* Task that draws a SpriteAnim at a screen position every frame. The first
 * 0x50 bytes belong to the task system (Task_Create). */
typedef struct SpriteAnimTask {
    u8 pad_00[0x50];
    SpriteAnim *sprite;
    int x;
    int y;
} SpriteAnimTask;

/* Same for a sprite drawn in 3D space. */
typedef struct SpriteAnim3dTask {
    u8 pad_00[0x50];
    SpriteAnim *sprite;
    VecFx32 pos;
} SpriteAnim3dTask;

extern SpriteAnim data_0229b080[SPRITE_ANIM_COUNT];

#endif
