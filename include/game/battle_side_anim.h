#ifndef GAME_BATTLE_SIDE_ANIM_H
#define GAME_BATTLE_SIDE_ANIM_H

#include "types.h"

/* Four 4-bit values; which one to read depends on the air/ground matchup of the two
 * sides (Battle_GetAirGroundMatchup). */
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

/* Three values read from the side's animation data when the fighter object is made. */
typedef struct BattleAnimStart {
    u8 pad_00[0x18];
    s16 unk_18;
    s16 unk_1a;
    s16 unk_1c;
} BattleAnimStart;

/* Animation data of one side of the ov002 battle screen. The lists hold one entry per
 * weapon (BattleSide.usesSecondary). */
typedef struct BattleSideAnim {
    BattleAnimNibbles *unk_00[2];
    BattleAnimNibbles *unk_08[2];
    u8 pad_10[0x1c - 0x10];
    BattleAnimBytes *unk_1c[2];
    u8 pad_24[0x44 - 0x24];
    BattleAnimStart *unk_44;
    u8 unk_48;                         /* zero: the fighters are made from the entry list, else one fighter */
} BattleSideAnim;

#endif
