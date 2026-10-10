#ifndef GAME_OV003_SPRITE_H
#define GAME_OV003_SPRITE_H

#include "types.h"

/* Where a sprite of Ov003_CreateRowSprite was made and which row it belongs to. */
typedef struct Ov003SpriteSpot {
    s16 x;
    s16 y;
    u8 row;                            /* compared with Ov003Unit.unk_12_3 */
} Ov003SpriteSpot;

/* A sprite from func_02027ab4 whose per-sprite data in overlay 3 starts at
 * 0x60 (Ov003_CreateRowSprite sets it, Ov003_CreateAnchoredSprite reads it). */
typedef struct Ov003Sprite {
    u8 pad_00[0x60];
    Ov003SpriteSpot spot;
} Ov003Sprite;

#endif
