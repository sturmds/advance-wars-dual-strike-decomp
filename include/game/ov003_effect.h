#ifndef GAME_OV003_EFFECT_H
#define GAME_OV003_EFFECT_H

#include "types.h"

#define OV003_EFFECTS_PER_OWNER 4

/* One of the 4 effects an owner can have at once in data_ov003_02369738. The
 * table has one group of 4 per Ov003Unit (groups 0 to 15), then one group per
 * Ov003Object (groups 16 and up). A free entry has lifetime 0. */
typedef struct Ov003Effect {
    s16 x;                             /* 128 per tile, plus 64 to centre it */
    s16 y;
    s16 velX;
    s16 velY;
    s16 unk_08;
    u8 owner;                          /* unit index, or object index | 0x80 */
    u8 kind;
    u8 lifetime;
    u8 unk_0d;
} Ov003Effect;

typedef struct Ov003EffectGroup {
    Ov003Effect effects[OV003_EFFECTS_PER_OWNER];
} Ov003EffectGroup;

/* 12-byte entry of data_ov003_02369bb8. */
typedef struct Ov003Object {
    u8 pad_00[4];
    s16 x;                             /* 128 per tile */
    s16 y;
    u8 kind : 4;
    u8 unk_08_4 : 4;
    u8 unk_09;
    u8 unk_0a;
    u8 pad_0b;
} Ov003Object;

typedef char Ov003Effect_size_check[sizeof(Ov003Effect) == 14 ? 1 : -1];
typedef char Ov003Object_size_check[sizeof(Ov003Object) == 12 ? 1 : -1];

#endif
