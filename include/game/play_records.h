#ifndef GAME_PLAY_RECORDS_H
#define GAME_PLAY_RECORDS_H

#include "types.h"
#include "game/co_data.h"
#include "game/co_save.h"

/* Best run of one Survival mode variant (data_022a7b80.mode, 0-5).
 * Survival_SaveRecord writes it at the end of a run. */
typedef struct SurvivalRecord {
    u8 co;                             /* CO id of the run, 0 when no record is saved (SurvivalRecord_Exists) */
    u8 tagCo;                          /* second CO id, 0 for none (SurvivalRecord_GetCo) */
    u8 pad_02[2];
    u32 value;                         /* data_022a7b80.budgetLeft at the end of the run */
} SurvivalRecord;
#include "game/war_room_records.h"

/* gPlayRecords (main bss, 0x02290718): saved settings, per-CO records
 * (coRecords, the Force Rank table Co_GetForceRankSetIndex reads) and
 * counters of what human players did, each capped at 999999 or 999999999. */
typedef struct PlayRecords {
    u32 points;                        /* spent in Hachi's shop (PlayRecords_SpendPoints); battles add to it (PlayRecords_AddCoins) */
    u32 unk_04;                        /* capped at 99999; PlayRecords_AddCoins adds to it with points and unk_384 */
    u8 unk_08;
    u8 unk_09;
    u8 unk_0a;
    u8 unk_0b;
    u8 unk_0c;
    u8 unk_0d;
    u8 unk_0e;                         /* copied to Unk_02080440_48.musicReady by a rules reset */
    u8 unk_0f;
    u8 unk_10;
    u8 unk_11;
    u8 unk_12;                         /* copied to MenuState.setup.unk_2e1 */
    u8 shopVisits;                     /* visits to Hachi's shop, up to 255 (Ov005_AddShopVisit) */
    u8 wallpaper;                      /* selected wallpaper, 0-30 (Wallpaper_LoadSelected); ov005 stores data_ov005_0236a48c[entry] (Ov005_SaveCoListEntry) */
    u8 unk_15;                         /* copied to MenuState.setup.unk_2e2 */
    u8 unk_16;                         /* aiBehaviour of slot 1 when gameMode is 1 (Players_ApplyBattleSetup) */
    u8 unk_17;                         /* aiBehaviour of every slot but the last when gameMode is 0 (Players_ApplyBattleSetup) */
    u8 pad_18[0x38 - 0x18];
    CoSaveRecord coRecords[CO_COUNT];  /* by CO id */
    SaveHalfwords16 unk_348[3];        /* one per profile; Save_LoadProfiles fills them from the save */
    u8 pad_378[0x37c - 0x378];
    u32 playFrames;                    /* frames counted by PlayRecords_CountFrame, capped by func_020242c0 */
    u32 unk_380;                       /* capped at 999999; Turn_AdvanceToNextPlayer adds 1 each time the turn order wraps past player 4, under conditions not yet understood */
    u32 unk_384;                       /* capped at 999999; PlayRecords_AddCoins adds to it */
    u32 unk_388;                       /* loaded from the save with a profile (Save_LoadProfile) */
    u32 unk_38c;
    u32 rank5Battles;                  /* battles a human ended with score rank 5 (likely S), capped at 999999 (PlayRecords_CountBattleRank) */
    u32 rank4Battles;
    u32 rank3Battles;
    u32 rank2Battles;
    u32 coPowersActivated;             /* CO powers activated by human players, capped at 999999 (UnitAction_ActivateCoPower) */
    u32 superPowersActivated;          /* same for super powers (UnitAction_ActivateSuperPower) */
    u32 tagPowersActivated;            /* same for tag powers (UnitAction_ActivateTagPower) */
    u32 directAttacks;                 /* attacks by human players' units with minRange 1, capped at 999999 (AttackTargetSelect_Finish) */
    u32 indirectAttacks;               /* same for units with a longer minRange */
    u32 capturesStarted;               /* captures begun by human players, capped at 999999 (UnitAction_Capture); name from the code */
    u32 capturesCompleted;             /* captures finished by human players, capped at 999999 */
    u32 coRankTotal;                   /* sum of Co_GetRank over CO ids 1-27 (PlayRecords_UpdateCoRankTotal) */
    u32 humanFundsSpent;               /* funds spent by human players, capped at 999999999 (Player_SpendFunds) */
    u32 humanDamageTaken;              /* funds value of hp lost by units of human players, capped at 999999999 (Unit_AddDamageTaken) */
    u32 silosLaunched;                 /* silo launches by human players, capped at 999999 (Silo_StartLaunch) */
    u32 unk_3cc;                       /* capped at 999999; UnitShowTask_RunScript adds 1 for a human player */
    u32 cannonHitsTaken;               /* cannon shots at human players' units, capped at 999999 (MapObjectFireTask_ShowNextShot) */
    u32 tilesMoved;                    /* tiles moved by human players' units, capped at 999999 (UnitMoveSelect_TryMoveTo) */
    u32 blackCannonsDestroyed;         /* map objects destroyed by human players' units, capped at 999999 (ObjectAttack_ShowHit) */
    u32 minicannonsDestroyed;
    u32 blackObelisksDestroyed;
    u32 blackCrystalsDestroyed;
    u32 pipeSeamsDestroyed;            /* also counts objects of kind 0 */
    u32 suppliesAndRepairs;            /* supply (action 6) and repair (action 24) actions by human players, capped at 999999; the name is a guess from those two callers */
    u32 unitsDeleted;                  /* capped at 999999; +1 per unit a human player deletes (Game_State15_PickUnitToDelete) */
    u32 unitsJoined;                   /* joins by human players, capped at 999999 (UnitAction_Join); name from the code, the meaning of the counter is a guess */
    u32 unitsBoarded;                  /* unit action 7 by human players, capped at 999999 (UnitAction_Board) */
    u32 unitsUnloaded;                 /* unloads by human players, capped at 999999 (ActingUnit_UnloadCargo) */
    u32 unitsSelected;                 /* units picked to move by human players, capped at 999999 (MapCursor_SelectTile) */
    u32 coSwaps;                       /* CO swaps by human players, capped at 999999 (UnitAction_SwapCo) */
    u32 unk_408;                       /* capped at 999999; +1 when a player loses with defeat code 2 on a human's turn (LinkPlayer_MarkDefeated) */
    u32 unk_40c;                       /* capped at 999999; +1 for defeat code 1 or 16 on a human's turn */
    u32 unk_410;                       /* capped at 999999; +1 when a human player is defeated outside Survival */
    u32 unitsSentToOtherMap;           /* unit action 26 by human players, capped at 999999 (UnitAction_MoveToOtherMap) */
    u32 survivalRunsCleared[3];        /* runs whose last map was won, for data_022a7b80.mode 0, 2, 1 in that order; capped at 999999 (Survival_FinishMap) */
    u32 survivalMapsWon[3];            /* maps won, for Survival mode 0 or 3, 2 or 5, 1 or 4 in that order; capped at 999999 */
    u32 combatClears[3];               /* Combat mode clears of the last stage by row 1, one counter per data_02160ac0->unk_84 (0-2), capped at 999999 (CombatResult_Init) */
    u32 unk_43c;                       /* capped at 999999; func_ov003_02361c7c adds the first 6 Ov003Row.unk_28 of the current slot's row */
    u32 unk_440;                       /* capped at 999999; func_ov003_02361c7c adds 1 when its row is not the current slot's */
    u32 unitsBought[27];               /* per unit type, capped at 999999 (CurrentPlayer_BuyUnit); tab list column, see TabList_GetUnitBought */
    u32 unitsDestroyed[27];            /* per unit type, capped at 999999: enemy units that human players destroyed */
    WarRoomRecord warRoomRecords[4][44][3]; /* the same bytes as gWarRoomRecords */
    u8 pad_d5c[0xe3c - 0xd5c];
    SurvivalRecord survivalRecords[6]; /* by Survival mode variant */
    u8 pad_e6c[0x105c - 0xe6c];
} PlayRecords;

extern PlayRecords gPlayRecords;

#endif
