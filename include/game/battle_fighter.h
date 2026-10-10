#ifndef GAME_BATTLE_FIGHTER_H
#define GAME_BATTLE_FIGHTER_H

#include "types.h"

/* A 0x78-byte object of the ov002 battle animation, one per side. */
typedef struct BattleFighter {
    u8 pad_00[0x44];
    u16 unk_44;                        /* copied to the animation tasks */
    u8 pad_46[0x59 - 0x46];
    u8 unk_59_0 : 4;
    u8 side : 1;                       /* 0 or 1 */
    u8 unk_59_5 : 3;
    u8 pad_5a[0x78 - 0x5a];
} BattleFighter;

#endif
