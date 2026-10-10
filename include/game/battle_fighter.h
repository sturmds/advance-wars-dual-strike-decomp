#ifndef GAME_BATTLE_FIGHTER_H
#define GAME_BATTLE_FIGHTER_H

#include "types.h"

#define BATTLE_SOUND_NONE 0xffff       /* an unused entry of BattleSounds */

/* Sound effect ids (Sound_PlaySe) of one fighter on the battle animation screen. */
typedef struct BattleSounds {
    u32 unk_00;                        /* Sound_PlaySubSe id of the attacker (func_ov002_0235fc68) */
    u32 unk_04;                        /* Battle_PlayFighterStartSe, BattleSide.usesSecondary of 0 */
    u32 unk_08;                        /* Battle_PlayFighterStartSe, otherwise */
    u32 unk_0c;                        /* Battle_PlayFighterReactSe, other fighter's state not 2 */
    u32 unk_10;                        /* Battle_PlayFighterReactSe, other fighter's state 2 */
    u32 unk_14;                        /* Battle_PlayFighterSe; without it Battle_PlayDefaultFighterSe */
} BattleSounds;

typedef struct BattlePoint {
    s16 x;
    s16 y;
} BattlePoint;

/* The table BattleFighter.shapes points to. */
typedef struct BattleFighterShapes {
    u8 pad_00[0x0e];
    BattlePoint points[1];             /* one per pose; length unknown */
} BattleFighterShapes;

/* A 0x78-byte object of the ov002 battle animation, one per side. */
typedef struct BattleFighter {
    u8 pad_00[0x18];
    const BattleFighterShapes *shapes;
    u8 pad_1c[0x30 - 0x1c];
    u8 unk_30[8];                      /* ring of flags; unk_5a_4 is the next to clear */
    s16 unk_38;
    s16 unk_3a;
    s16 unk_3c;
    s16 unk_3e;
    s16 unk_40;
    s16 unk_42;
    u16 unk_44;                        /* copied to the animation tasks */
    s16 unk_46;
    u8 unk_48;
    u8 unk_49[3];                      /* indexed by BattleSide.usesSecondary of the same side */
    const BattleSounds *sounds;
    u8 pad_50[0x57 - 0x50];
    u8 unk_57;                         /* frame counter of the motion steps */
    u8 unk_58_0 : 1;
    u8 unk_58_1 : 7;
    u8 unk_59_0 : 3;
    u8 active : 1;                     /* the motion steps do nothing while clear */
    u8 side : 1;                       /* 0 or 1 */
    u8 unk_59_5 : 3;                   /* index into the side's BattleEntry list */
    u8 unk_5a_0 : 4;
    u8 unk_5a_4 : 4;                   /* index into unk_30, 0 to 7 */
    u8 pad_5b[0x78 - 0x5b];
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
