#ifndef GAME_UNK_02290118_H
#define GAME_UNK_02290118_H

#include "types.h"
#include "game/co_data.h"

/* One campaign map record in data_02290118.mapRecords. The list ends at the
 * first entry with days 0. */
typedef struct MapRecord {
    u32 mapIndex : 8;                  /* map id - 224 */
    u32 days : 12;
    u32 score : 12;
} MapRecord;

/* The same memory as data_02290118.mapRecords. */
extern MapRecord data_022901a4[28];

/* data_02290118 (main bss): saved progress. Only the part the campaign
 * world map reads is described. */
typedef struct Unk_02290118 {
    u8 unk_00;                         /* picks the Force Rank profile Save_LoadProgress reads from the save buffer */
    u8 unk_01;
    u8 hardMode;                       /* 1: the campaign is in hard mode (Campaign_IsHardMode) */
    u8 pad_03;
    u8 unk_04;
    u8 unk_05;
    u16 unk_06;
    u16 unk_08;
    u16 unk_0a;
    u16 unk_0c;
    u16 unk_0e;
    u8 pad_010[0x8c - 0x10];
    u32 unk_8c;                        /* bits 8-19 are cleared when a campaign starts (Campaign_ResetProgress) */
    u8 pad_090[0x11a - 0x90];
    u8 coScore[CO_COUNT];              /* per CO id; the world map shows the three highest */
} Unk_02290118;

extern Unk_02290118 data_02290118;

#endif
