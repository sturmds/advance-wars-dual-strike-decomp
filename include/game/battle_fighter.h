#ifndef GAME_BATTLE_FIGHTER_H
#define GAME_BATTLE_FIGHTER_H

#include "types.h"

#define BATTLE_SOUND_NONE 0xffff       /* an unused entry of BattleSounds */

/* Sound effect ids (Sound_PlaySe) of one fighter on the battle animation screen. */
typedef struct BattleSounds {
    u32 unk_00;
    u32 unk_04;                        /* played for a side with no CO */
    u32 unk_08;                        /* played for a side with a CO */
    u32 unk_0c;
    u32 unk_10;
    u32 unk_14;
} BattleSounds;

/* A 0x78-byte object of the ov002 battle animation, one per side. */
typedef struct BattleFighter {
    u8 pad_00[0x44];
    u16 unk_44;                        /* copied to the animation tasks */
    u8 pad_46[0x48 - 0x46];
    u8 unk_48;
    u8 unk_49[3];                      /* indexed by BattleSide.usesSecondary of the same side */
    const BattleSounds *sounds;
    u8 pad_50[0x59 - 0x50];
    u8 unk_59_0 : 4;
    u8 side : 1;                       /* 0 or 1 */
    u8 unk_59_5 : 3;
    u8 pad_5a[0x78 - 0x5a];
} BattleFighter;

/* Task of the ov002 battle animation that Battle_ReleaseSideTasks counts frames on. */
typedef struct BattleCounterTask {
    u8 pad_00[0x3c];
    s16 unk_3c;
    u8 pad_3e[0x42 - 0x3e];
    s16 unk_42;
    u8 pad_44[0x5d - 0x44];
    u8 unk_5d;                         /* zero: the task is done */
} BattleCounterTask;

#endif
