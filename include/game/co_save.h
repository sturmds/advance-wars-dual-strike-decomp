#ifndef GAME_CO_SAVE_H
#define GAME_CO_SAVE_H

#include "types.h"

/* Skill ids of one saved Force Rank set, ended by 0 when fewer than 4. */
typedef u8 ForceRankSet[4];

/* CO picks of one game mode, kept between maps: a front and a tag CO id per
 * player 0-4 (Map_SaveCoPicks). */
typedef u8 CoPicks[5][2];

/* Saved progress of one CO. The records sit in PlayRecords.coRecords,
 * indexed by CO id. */
typedef struct CoSaveRecord {
  u32 experience : 19; /* Co_SetExperience caps it at 100000 */
  u32 forceRankSetByPlayer
      : 8; /* 2 bits per player 1-4: which Force Rank set the CO uses */
  u32 paletteVariant : 3;    /* 0-7, see Co_LoadPalette */
  u32 pictureVariant : 1;    /* alternate picture */
  u32 available : 1;         /* Co_SetAvailable, Co_IsAvailable; name a guess */
  ForceRankSet setMode1;     /* ForceRank_GetSavedSet mode 1 */
  ForceRankSet setMode3;     /* mode 3 */
  ForceRankSet setsMode2[4]; /* mode 2 */
} CoSaveRecord;

/* Per-profile blocks in the save record and the live data. Save_LoadProfiles
 * copies each as a unit; the wrappers make the compiler copy by halfword and
 * by byte, as the original does. */
typedef struct SaveHalfwords16 {
  u16 value[8];
} SaveHalfwords16;

typedef struct SaveBytes30 {
  u8 value[30];
} SaveBytes30;

#endif
