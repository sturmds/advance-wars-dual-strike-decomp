#ifndef GAME_BATTLE_HIT_LIST_H
#define GAME_BATTLE_HIT_LIST_H

#include "types.h"

/* One byte of flags per entry of a BattleHitList. */
typedef struct BattleHitFlags {
    u8 unk_0 : 3;
    u8 unk_3 : 3;
    u8 unk_6 : 1;
    u8 unk_7 : 1;
} BattleHitFlags;

/* A list of up to five entries of the ov002 battle animation, kept sorted by key
 * (the name is a guess). */
typedef struct BattleHitList {
    BattleHitFlags flags[6];
    u16 key[5];
    u8 pad_10;
    u8 count;
} BattleHitList;

#endif
