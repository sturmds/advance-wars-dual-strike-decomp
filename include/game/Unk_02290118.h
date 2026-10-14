#ifndef GAME_UNK_02290118_H
#define GAME_UNK_02290118_H

#include "types.h"
#include "game/co_data.h"
#include "game/co_save.h"

/* One campaign map record. The 28 records start at data_02290118.unk_8c and
 * end at the first entry with days 0. */
typedef struct MapRecord {
  u32 mapIndex : 8; /* map id - 224 */
  u32 days : 12;
  u32 score : 12;
} MapRecord;

/* The same memory as the records at data_02290118.unk_8c. */
extern MapRecord gCampaignMapRecords[];

/* data_02290118 (main bss): saved progress. Only the part the campaign
 * world map reads is described. */
typedef struct Unk_02290118 {
  u8 unk_00; /* picks the Force Rank profile Save_LoadProgress reads from the save buffer */
  u8 unk_01;
  u8 hardMode; /* 1: the campaign is in hard mode (Campaign_IsHardMode) */
  u8 unk_03; /* counts the times the profile was stored, up to 255 (Save_StoreProfile) */
  u8 unk_04;
  u8 unk_05;
  u16 unk_06;
  u16 unk_08;
  u16 unk_0a;
  u16 unk_0c;
  u16 unk_0e;
  ForceRankSet
      forceRankSets[CO_COUNT]; /* mode 0, by CO id (ForceRank_GetSavedSet) */
  CoPicks coPicksMode0;        /* gCoPicksMode0 */
  u8 pad_08a[0x8c - 0x8a];
  union {
    u32 unk_8c; /* bits 8-19 are cleared when a campaign starts (Campaign_ResetProgress) */
    MapRecord records[28];
  };
  u8 pad_0fc[0x11a - 0xfc];
  u8 coScore[CO_COUNT]; /* per CO id; the world map shows the three highest */
  u8 flagsHigh[8];      /* gGameFlagsHigh: game flag ids 95-158 */
  u8 unk_13e[0x40];     /* data_02290256: Save_ResetProgress clears it */
  u8 pad_17e[0x180 - 0x17e];
} Unk_02290118;

typedef char Unk_02290118_size_check[sizeof(Unk_02290118) == 0x180 ? 1 : -1];

extern Unk_02290118 data_02290118;

/* The saved copy of the first 16 bytes of Unk_02290118 for one profile.
 * data_02290a60 holds three, in the same memory as gPlayRecords.unk_348;
 * Save_LoadProfile copies one over the start of data_02290118. */
typedef struct ProfileHeader {
  u8 unk_00;
  u8 unk_01;
  u8 hardMode;
  u8 inUse; /* nonzero: the profile is in use (Profile_IsInUse) */
  u8 unk_04;
  u8 unk_05;
  u16 unk_06;
  u16 unk_08;
  u16 unk_0a;
  u16 unk_0c;
  u16 unk_0e;
} ProfileHeader;

extern ProfileHeader data_02290a60[];

#endif
