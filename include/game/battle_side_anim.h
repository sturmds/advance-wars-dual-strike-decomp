#ifndef GAME_BATTLE_SIDE_ANIM_H
#define GAME_BATTLE_SIDE_ANIM_H

#include "types.h"

/* Two 4-bit values in a byte, which of them to read depends on the matchup of the
 * two sides (Battle matchup: both on the ground, one in the air, ...). */
typedef struct BattleAnimNibbles {
    u8 pad_00[0x29];
    u8 unk_29_0 : 4;
    u8 unk_29_4 : 4;
    u8 unk_2a_0 : 4;
    u8 unk_2a_4 : 4;
} BattleAnimNibbles;

typedef struct BattleAnimBytes {
    u8 pad_00[8];
    u8 unk_08_0 : 4;
    u8 unk_08_4 : 4;
    u8 unk_09;
} BattleAnimBytes;

/* Animation data of one side of the ov002 battle screen. The lists hold one entry per
 * weapon (BattleSide.usesSecondary). */
typedef struct BattleSideAnim {
    BattleAnimNibbles *unk_00[2];
    BattleAnimNibbles *unk_08[2];
    u8 pad_10[0x1c - 0x10];
    BattleAnimBytes *unk_1c[2];
} BattleSideAnim;

#endif
