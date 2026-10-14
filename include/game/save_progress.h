#ifndef GAME_SAVE_PROGRESS_H
#define GAME_SAVE_PROGRESS_H

#include "types.h"
#include "game/co_data.h"
#include "game/co_save.h"
#include "game/design_map.h"
#include "game/play_records.h"
#include "game/Unk_022a7b80.h"
#include "game/Unk_0229e148.h"

/* data_02291774 (main bss): the buffer that Save_LoadRecord(0, ...) fills.
 * Save_LoadProgress copies its CO records and Force Rank sets into the live
 * progress data. */
typedef struct SaveProgress {
  u8 pad_00[0x58];
  u32 unk_58; /* Save_LoadProgress passes it to PlayRecords_CapPoints */
  u32 unk_5c; /* passed to func_02017c88 */
  u8 pad_60[0x90 - 0x60];
  CoSaveRecord coRecords[CO_COUNT]; /* by CO id */
  SaveHalfwords16 unk_3a0
      [3]; /* one per profile; Save_LoadProfiles copies them to gPlayRecords */
  u8 pad_3d0[0x10c4 - 0x3d0];
  ForceRankSet profileSets
      [1][96]; /* [Unk_02290118.unk_00][CO id]; more profiles may follow */
  u8 pad_1244[0x1534 - 0x1244];
  SaveBytes30 unk_1534
      [3]; /* one per profile; Save_LoadProfiles copies them to data_022900bc */
} SaveProgress;

/* Blocks of save record 0, each copied as one unit. */
typedef struct SaveGameFlagsMid {
  u8 value[86]; /* gGameFlagsMid */
} SaveGameFlagsMid;

/* Game progress flags 32-94 from byte 0 and the map flags (one bit per map
 * id) from byte 10. The backup holds them during a link battle
 * (GameFlagsMid_Backup). */
extern SaveGameFlagsMid gGameFlagsMid;
extern SaveGameFlagsMid gGameFlagsMidBackup;

typedef struct SaveProfileBlock {
  u32 sets[96]; /* data_02290298[profile] */
} SaveProfileBlock;

typedef struct SaveLinkReactionTexts {
  u8 value[198]; /* data_0229da8c, see LinkReaction_LoadTexts */
} SaveLinkReactionTexts;

/* Save record 0 as Save_PackProgress and Save_UnpackProgress see it: the
 * live data that the record keeps, in order. 0x1684 bytes. */
typedef struct SaveProgressBlocks {
  SaveGameFlagsMid gameFlagsMid;
  u8 pad_56[0x58 - 0x56];
  PlayRecords playRecords;
  SaveProfileBlock profileBlocks[3];
  SaveBytes30 unk_1534[3]; /* data_022900bc */
  u8 pad_158e[0x1590 - 0x158e];
  Unk_022a7b80 unk_1590; /* data_022a7b80 */
  Unk_0229e148 unk_15a4; /* data_0229e148 */
  SaveLinkReactionTexts linkReactionTexts;
  u8 pad_1682[0x1684 - 0x1682];
} SaveProgressBlocks;

typedef char SaveProgressBlocks_size_check[sizeof(SaveProgressBlocks) == 0x1684
                                               ? 1
                                               : -1];

/* The buffer Save_Init hands to the save code (gSaveBuffer). Each record is
 * loaded into it and packed in it before it is written: record 0 as
 * SaveProgress and SaveProgressBlocks (0x1684 bytes), the battle saves as
 * BattleSaveRecord (0x2238), the design maps as DesignMapRecord. Those run
 * past SaveProgress's 0x1590 bytes. bytes covers the 0x6000 bytes up to the
 * next variable; no code names the part past 0x2238. */
typedef union SaveRecordBuffer {
  SaveProgress progress;
  SaveProgressBlocks blocks;
  DesignMapRecord designMaps[2]; /* data_02291e98 names the second */
  u8 bytes[0x6000];
} SaveRecordBuffer;

extern SaveRecordBuffer data_02291774;

#endif
