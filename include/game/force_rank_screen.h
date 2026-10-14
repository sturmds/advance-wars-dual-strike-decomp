#ifndef GAME_FORCE_RANK_SCREEN_H
#define GAME_FORCE_RANK_SCREEN_H

#include "types.h"
#include "game/text_ref.h"

/* One entry of the Force Rank table in ov000 (Ov000_gForceRankTable),
 * indexed by skill id. */
typedef struct ForceRankInfo {
  u8 column; /* 0: not listed; 1-9 rank needed; 10 special (game flags) */
  u8 pad_01[3];
  TextRef name;        /* group 0x7c00 */
  TextRef description; /* group 0x7b00 */
} ForceRankInfo;

/* The Force Rank selection screen lays its entries out in 12 columns
 * (0-11) of up to 6 rows. A cursor holds the column in bits 3 and up and
 * the row in bits 0-2. */
#define FORCE_RANK_COLUMN_SPECIAL 10
#define FORCE_RANK_COLUMN_NONE 11 /* holds skill id 0 only */
#define FORCE_RANK_COLUMN_COUNT 12

/* Skill slots of a CO: the skills it can equip at once. */
#define FORCE_RANK_SLOT_COUNT 4

/* One column of the skills screen: the skill ids listed in it. */
typedef struct ForceRankColumn {
  u8 skills[6];
  u8 count; /* 0: the cursor skips the column */
} ForceRankColumn;

/* Work area of the Force Rank selection screen, through gForceRankScreen. */
typedef struct ForceRankScreen {
  ForceRankColumn columns[FORCE_RANK_COLUMN_COUNT];
  u8 chosenSkills
      [FORCE_RANK_SLOT_COUNT]; /* skill id picked in each slot; 0: empty */
  u8 pad_58[0x59 - 0x58];
  u8 cycleColors
      [0x20]; /* 16 colors, unaligned; the slot cursor shows one every 4 frames */
  u8 co;      /* CO whose skills are being edited */
  u8 rank; /* the CO's rank: columns up to it are listed, and it opens min(4, rank) skill slots */
  u8 selectedSlot; /* skill slot the cursor is on; L and R cycle it */
  u8 cursorRow;    /* row and column of the selected skill button */
  u8 row;          /* row the cursor keeps when it changes column */
  u8 cursorColumn;
  u8 mode; /* which saved skill lists: ForceRank_SaveSets's mode */
  u8 set;  /* skill set of the CO being edited */
} ForceRankScreen;

extern ForceRankScreen* gForceRankScreen;
extern ForceRankInfo Ov000_gForceRankTable[];

#endif
