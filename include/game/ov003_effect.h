#ifndef GAME_OV003_EFFECT_H
#define GAME_OV003_EFFECT_H

#include "types.h"

#define OV003_EFFECTS_PER_OWNER 4

/* One of the 4 effects an owner can have at once in data_ov003_02369738. The
 * table has one group of 4 per Ov003Unit (groups 0 to 15), then one group per
 * Ov003Marker (groups 16 and up). A free entry has lifetime 0. */
typedef struct Ov003Effect {
  s16 x; /* 128 per tile, plus 64 to centre it */
  s16 y;
  s16 velX;
  s16 velY;
  s16 unk_08;
  u8 owner; /* unit index, or marker index | 0x80 */
  u8 kind;
  u8 lifetime;
  u8 row; /* row (team) that fired it (Ov003Object.row) */
} Ov003Effect;

typedef struct Ov003EffectGroup {
  Ov003Effect effects[OV003_EFFECTS_PER_OWNER];
} Ov003EffectGroup;

typedef char Ov003Effect_size_check[sizeof(Ov003Effect) == 14 ? 1 : -1];

#endif
