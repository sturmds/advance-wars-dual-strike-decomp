#ifndef GAME_BATTLE_ACTOR_H
#define GAME_BATTLE_ACTOR_H

#include "types.h"

/* A sprite task of the battle animation screen (ov002): the five per side
 * that gMenuState.battle.unk_310 points to. The fields are the ones read so
 * far. */
typedef struct BattleActor {
    u8 pad_00[0x2c];
    struct Task *shadow;               /* follows the actor; destroyed when it ends */
    u8 pad_30[0x46 - 0x30];
    s16 unk_46;
    u8 pad_48[0x54 - 0x48];
    u8 unk_54;
    u8 unk_55;
    u8 pad_56[0x59 - 0x56];
    u8 unk_59_0 : 1;
    u8 hasShadow : 1;
    u8 unk_59_2 : 1;
    u8 unk_59_3 : 1;
    u8 side : 1;                       /* which side of the screen */
} BattleActor;

#endif
