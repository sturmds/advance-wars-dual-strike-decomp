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

/* What BattleScreenWork.unk_27c points to, one per side of the battle in ov002. */
typedef struct BattleSideData {
    u8 pad_00[0x10];
    BattleSideData_10 *unk_10;
    u8 pad_14[4];
    const BattleCellPos *cellPositions; /* by cell */
    u8 pad_1c[0x48 - 0x1c];
    u8 unk_48;                         /* nonzero: BattleCellTask steps are all 0 for the side */
    u8 unk_49[1];                      /* length unknown; indexed by byte 9 of the side's 12-byte record in the scene */
} BattleSideData;

#endif
