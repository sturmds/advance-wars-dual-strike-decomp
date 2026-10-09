#ifndef GAME_PLAY_RECORDS_H
#define GAME_PLAY_RECORDS_H

#include "types.h"
#include "game/co_data.h"
#include "game/co_save.h"

/* gPlayRecords (main bss, 0x02290718): saved settings, per-CO records
 * (coRecords, the Force Rank table Co_GetForceRankSetIndex reads) and
 * counters of what human players did, each capped at 999999 or 999999999. */
typedef struct PlayRecords {
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
    SaveHalfwords16 unk_348[3];        /* one per profile; Save_LoadProfiles fills them from the save */
    u8 pad_378[0x380 - 0x378];
    u32 unk_380;                       /* capped at 999999; Turn_AdvanceToNextPlayer adds 1 each time the turn order wraps past player 4, under conditions not yet understood */
    u8 pad_384[0x3a0 - 0x384];
    u32 coPowersActivated;             /* CO powers activated by human players, capped at 999999 (UnitAction_ActivateCoPower) */
    u32 superPowersActivated;          /* same for super powers (UnitAction_ActivateSuperPower) */
    u32 tagPowersActivated;            /* same for tag powers (UnitAction_ActivateTagPower) */
    u8 pad_3ac[0x3b4 - 0x3ac];
    u32 capturesStarted;               /* captures begun by human players, capped at 999999 (UnitAction_Capture); name from the code */
    u32 capturesCompleted;             /* captures finished by human players, capped at 999999 */
    u8 pad_3bc[0x3c0 - 0x3bc];
    u32 humanFundsSpent;               /* funds spent by human players, capped at 999999999 (Player_SpendFunds) */
    u32 humanDamageTaken;              /* funds value of hp lost by units of human players, capped at 999999999 (Unit_AddDamageTaken) */
    u32 silosLaunched;                 /* silo launches by human players, capped at 999999 (Silo_StartLaunch) */
    u8 pad_3cc[0x3ec - 0x3cc];
    u32 suppliesAndRepairs;            /* supply (action 6) and repair (action 24) actions by human players, capped at 999999; the name is a guess from those two callers */
    u8 pad_3f0[0x3f4 - 0x3f0];
    u32 unitsJoined;                   /* joins by human players, capped at 999999 (UnitAction_Join); name from the code, the meaning of the counter is a guess */
    u32 unitsBoarded;                  /* unit action 7 by human players, capped at 999999 (UnitAction_Board) */
    u8 pad_3fc[0x404 - 0x3fc];
    u32 coSwaps;                       /* CO swaps by human players, capped at 999999 (UnitAction_SwapCo) */
    u8 pad_408[0x414 - 0x408];
    u32 unitsSentToOtherMap;           /* unit action 26 by human players, capped at 999999 (UnitAction_MoveToOtherMap) */
} PlayRecords;

extern PlayRecords gPlayRecords;

#endif
