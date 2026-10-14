#ifndef GAME_OV003_SPRITE_H
#define GAME_OV003_SPRITE_H

#include "types.h"
#include "game/ov003_unit.h"

/* Where a sprite of Ov003_CreateRowSprite was made and which row it belongs to. */
typedef struct Ov003SpriteSpot {
  s16 x;
  s16 y;
  u8 row; /* compared with Ov003Unit.row */
} Ov003SpriteSpot;

/* A sprite from Sprite_CreateTask whose per-sprite data in overlay 3 starts at
 * 0x60 (Ov003_CreateRowSprite sets it, Ov003_CreateAnchoredSprite reads it). */
typedef struct Ov003Sprite {
  u8 pad_00[0x60];
  Ov003SpriteSpot spot;
} Ov003Sprite;

/* A sprite from Ov003_CreateUnitSprite that follows one unit; its callback is
 * func_ov003_02364300. */
typedef struct Ov003UnitSprite {
  u8 pad_00[0x60];
  Ov003Unit* unit;
} Ov003UnitSprite;

/* An entry of the part lists that overlay 3's sprite frame lists point to
 * (data_ov003_023657b4 and the others reached through TableSpriteTask.unk_30):
 * two words. A list ends with an entry of zeros. */
typedef struct Ov003SpritePart {
  u32 unk_00;
  u32 unk_04;
} Ov003SpritePart;

#endif
