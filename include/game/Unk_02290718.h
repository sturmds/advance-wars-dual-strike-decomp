#ifndef GAME_UNK_02290718_H
#define GAME_UNK_02290718_H

#include "types.h"
#include "game/co_data.h"
#include "game/co_save.h"

/* data_02290718 (main bss), 0x38 bytes before the Force Rank table
 * that Co_GetForceRankSetIndex reads. */
typedef struct Unk_02290718 {
    u8 pad_00[0x0e];
    u8 unk_0e;                         /* copied to Unk_02080440_48.musicReady by a rules reset */
    u8 unk_0f;
    u8 pad_10[0x12 - 0x10];
    u8 unk_12;                         /* copied to MenuState.setup.unk_2e1 */
    u8 pad_13[0x15 - 0x13];
    u8 unk_15;                         /* copied to MenuState.setup.unk_2e2 */
    u8 unk_16;                         /* aiBehaviour of slot 1 when gameMode is 1 (Players_ApplyBattleSetup) */
    u8 unk_17;                         /* aiBehaviour of every slot but the last when gameMode is 0 (Players_ApplyBattleSetup) */
    u8 pad_18[0x38 - 0x18];
    CoSaveRecord coRecords[CO_COUNT];  /* by CO id */
    u8 pad_after_records[0x380 - 0x38 - CO_COUNT * sizeof(CoSaveRecord)];
    u32 unk_380;                       /* capped at 999999; Turn_AdvanceToNextPlayer adds 1 each time the turn order wraps past player 4, under conditions not yet understood */
    u8 pad_384[0x3a0 - 0x384];
    u32 coPowersActivated;             /* CO powers activated by human players, capped at 999999 (UnitAction_ActivateCoPower) */
    u32 superPowersActivated;          /* same for super powers (UnitAction_ActivateSuperPower) */
    u32 tagPowersActivated;            /* same for tag powers (func_020dd404) */
    u8 pad_3ac[0x3b4 - 0x3ac];
    u32 capturesStarted;               /* captures begun by human players, capped at 999999 (UnitAction_Capture); name from the code */
    u32 capturesCompleted;             /* captures finished by human players, capped at 999999 */
    u8 pad_3bc[0x3c0 - 0x3bc];
    u32 humanFundsSpent;               /* funds spent by human players, capped at 999999999 (Player_SpendFunds) */
    u32 humanDamageTaken;              /* funds value of hp lost by units of human players, capped at 999999999 (Unit_AddDamageTaken) */
    u8 pad_3c8[0x3ec - 0x3c8];
    u32 unk_3ec;                       /* counted for human players by func_020ddc2c and func_020ddcb8, capped at 999999; meaning unknown */
    u8 pad_3f0[0x3f4 - 0x3f0];
    u32 unitsJoined;                   /* joins by human players, capped at 999999 (UnitAction_Join); name from the code, the meaning of the counter is a guess */
} Unk_02290718;

extern Unk_02290718 data_02290718;

#endif
