#ifndef GAME_BATTLESIDEDATA_H
#define GAME_BATTLESIDEDATA_H

#include "types.h"

/* Reached through BattleSideData.unk_10. mwcc packs bit fields from the low end. */
typedef struct BattleSideData_10 {
    u8 pad_00[0x29];
    u8 unk_29_lo : 4;
    u8 sameAltitude : 4;               /* value for Battle_GetAirGroundMatchup 0 */
    u8 groundVsAir : 4;                /* matchup 1 */
    u8 airVsGround : 4;                /* matchup 2 */
} BattleSideData_10;

/* Position of one HP cell on the battle screen. */
typedef struct BattleCellPos {
    s16 x;
    s16 y;
    s16 z;
} BattleCellPos;

/* Reached through BattleSideData.weapons. */
typedef struct BattleSideWeapon {
    u8 pad_00[0x28];
    u8 spriteIndex : 4;                /* index argument of BattleSpriteTask_Create for the HP cells */
    u8 unk_28_hi : 4;
} BattleSideWeapon;

/* What BattleScreenWork.unk_27c points to, one per side of the battle in ov002. */
typedef struct BattleSideData {
    const BattleSideWeapon *weapons[2]; /* by BattleSide.usesSecondary */
    u8 pad_08[8];
    BattleSideData_10 *unk_10;
    u8 pad_14[4];
    const BattleCellPos *cellPositions; /* by cell */
    u8 pad_1c[0x48 - 0x1c];
    u8 unk_48;                         /* 0: the cells step with the hit list (BattleCells_RunSequence); else the side runs a fighter task */
    u8 unk_49[1];                      /* length unknown; indexed by byte 9 of the side's 12-byte record in the scene */
} BattleSideData;

#endif
