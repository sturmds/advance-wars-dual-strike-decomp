#ifndef GAME_SAVE_PROGRESS_H
#define GAME_SAVE_PROGRESS_H

#include "types.h"
#include "game/co_data.h"
#include "game/battle_save.h"
#include "game/co_save.h"
#include "game/design_map.h"
#include "game/play_records.h"
#include "game/reaction_text_menu_task.h"
#include "game/Unk_022a7b80.h"
#include "game/Unk_0229e148.h"
#include "game/Unk_02290118.h"

/* Blocks of save record 0, each copied as one unit. */
typedef struct SaveGameFlagsMid {
  u8 value[86]; /* gGameFlagsMid */
} SaveGameFlagsMid;

/* Game progress flags 32-94 from byte 0 and the map flags (one bit per map
 * id) from byte 10. The backup holds them during a link battle
 * (GameFlagsMid_Backup). */
extern SaveGameFlagsMid gGameFlagsMid;
extern SaveGameFlagsMid gGameFlagsMidBackup;

/* A copy of data_02290118 for one profile (data_02290298[profile]). */
typedef struct SaveProfileBlock {
  Unk_02290118 progress;
} SaveProfileBlock;

/* Profiles the save keeps, and their live copies. */
#define SAVE_PROFILE_COUNT 3
extern SaveProfileBlock data_02290298[SAVE_PROFILE_COUNT];
extern SaveBytes30 data_022900bc[SAVE_PROFILE_COUNT];

/* Save record 0: the live data that the record keeps, in order. Save_PackProgress
 * and Save_UnpackProgress copy it whole; Save_LoadRecord(0, ...) reads it into
 * data_02291774, from which Save_LoadProgress, Save_LoadProfiles and
 * Save_LoadProfile copy parts. 0x1684 bytes with retail's 28 COs. */
typedef struct SaveProgressBlocks {
  SaveGameFlagsMid gameFlagsMid;
  u8 pad_56[0x58 - 0x56];
  PlayRecords playRecords;
  SaveProfileBlock profileBlocks[SAVE_PROFILE_COUNT];
  SaveBytes30 unk_1534[3]; /* data_022900bc */
  u8 pad_158e[0x1590 - 0x158e];
  Unk_022a7b80 unk_1590; /* data_022a7b80 */
  Unk_0229e148 unk_15a4; /* data_0229e148 */
  SaveLinkReactionTexts linkReactionTexts;
  u8 pad_1682[0x1684 - 0x1682];
} SaveProgressBlocks;

/* Each CO adds a CoSaveRecord to playRecords, and to each profile block the
 * growth of Unk_02290118. */
typedef char SaveProgressBlocks_size_check
    [sizeof(SaveProgressBlocks) ==
             0x1684 + (CO_COUNT - 28) * sizeof(CoSaveRecord) +
                 SAVE_PROFILE_COUNT * (sizeof(Unk_02290118) - 0x180)
         ? 1
         : -1];

/* The buffer Save_Init hands to the save code (gSaveBuffer). Each record is
 * loaded into it and packed in it before it is written: record 0 as
 * SaveProgressBlocks, the battle saves as BattleSaveRecord (0x2238), the
 * design maps as DesignMapRecord. bytes covers the 0x6000 bytes up to the
 * next variable; no code names the part past 0x2238. */
typedef union SaveRecordBuffer {
  SaveProgressBlocks blocks;
  BattleSaveRecord battle;
  DesignMapRecord designMaps[2]; /* data_02291e98 names the second */
  u8 bytes[0x6000];
} SaveRecordBuffer;

extern SaveRecordBuffer data_02291774;

#endif
