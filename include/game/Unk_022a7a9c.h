#ifndef GAME_UNK_022A7A9C_H
#define GAME_UNK_022A7A9C_H

#include "types.h"

/* data_022a7a9c (main bss). ScoreList_AddRow reads these bytes as points
 * (speed, power, technique) or as counts it multiplies into points (0x08-0x0d). */
typedef struct Unk_022a7a9c {
    u8 speed;                          /* BattleScore_Speed of player */
    u8 power;                          /* BattleScore_Power */
    u8 technique;                      /* BattleScore_Technique */
    u8 rank;                           /* Score_ToRank_020e5428 of score; ResultPath_DrawAll starts its sprites at path step 20 + 35 * rank */
    u8 player;                         /* slot scored: Player_GetCurrentOrFirstInGame at the end of the battle; WinQuote_Init copies it to its task's unk_64 */
    u8 pad_05;
    u16 score;                         /* saved by BattleRecord_SaveWin as a Campaign or War Room record */
    u8 unk_08;
    u8 unk_09;
    u8 unk_0a;
    u8 unk_0b;
    u8 unk_0c;
    u8 unk_0d;
    u8 pad_0e[0x10 - 0x0e];
    s32 unk_10;                        /* entry 0: points the win/lose screen shows */
    s32 totalPoints;                   /* score plus bonus points (BattleScore_Compute); BattleEnd_AwardCoins adds totalPoints - previousBest to the coins */
    s32 previousBest;                  /* campaign: the profile's earlier best on this map, from ProfileHeaders_UpdateBest */
    u16 frontCo;                       /* CO id in bits 0-6 (WinLoseScreen_Init) */
    u16 backCo;                        /* CO id in bits 0-6; 0: no tag partner */
    u8 pad_20[0x28 - 0x20];            /* WinLoseScreen_Init indexes 0x28-byte entries by player slot */
} Unk_022a7a9c;

extern Unk_022a7a9c data_022a7a9c;

/* 0x28-byte records from data_022a7ab8. The low 7 bits of each of the
 * first two halfwords hold a CO id; the second halfword has its own
 * symbol, data_022a7aba, so it is read through that. */
typedef struct Unk_022a7ab8 {
    u16 unk_00;
    u16 unk_02;                        /* back CO; read through data_022a7aba */
    u16 rankBefore[2];                 /* per CO slot: rank before this battle's experience */
    u16 rankAfter[2];                  /* rank after it */
    u16 expBonus[2];                   /* EXP_BONUS_* */
    s32 expInRank[2];                  /* experience past the last full rank (0-999) */
    s32 expGain[2];
    u8 propertyCounts[5];              /* cities, bases, airports, ports, Com Towers owned at the end */
    u8 unitCount;                      /* units on the map at the end, carried units included */
    u8 armyColour;                     /* Player.armyColour */
    u8 pad_27;
} Unk_022a7ab8;

/* Experience multipliers of a result row (ExpBonus_Scale). */
#define EXP_BONUS_X1   0
#define EXP_BONUS_X1_5 1
#define EXP_BONUS_X2   2
#define EXP_BONUS_X2_5 3

/* data_022a7a9c seen whole: Unk_022a7a9c covers its first 0x28 bytes, and
 * the per-slot rows start at 0x1c (data_022a7ab8). */
typedef struct BattleResults {
    u8 pad_00[0x14];
    s32 expGain;                       /* experience each human player's COs gain, before ExpBonus_Scale */
    s32 expTaken;                      /* experience taken from them first, before ExpBonus_Scale */
    Unk_022a7ab8 rows[5];              /* by player slot */
} BattleResults;

extern Unk_022a7ab8 data_022a7ab8[];
extern Unk_022a7ab8 data_022a7aba[];  /* the same records, from their halfword at +2 */

#endif
