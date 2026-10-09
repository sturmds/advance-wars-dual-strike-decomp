#ifndef GAME_FORCE_RANK_SCREEN_H
#define GAME_FORCE_RANK_SCREEN_H

#include "types.h"
#include "game/text_ref.h"

/* One entry of the Force Rank table in ov000 (Ov000_gForceRankTable),
 * indexed by skill id. */
typedef struct ForceRankInfo {
    u8 column;                         /* 0: not listed; 1-9 rank needed; 10 special (game flags) */
    u8 pad_01[3];
    TextRef name;                      /* group 0x7c00 */
    TextRef description;               /* group 0x7b00 */
} ForceRankInfo;

/* The Force Rank selection screen lays its entries out in 12 columns
 * (0-11) of up to 6 rows. A cursor holds the column in bits 3 and up and
 * the row in bits 0-2. */
#define FORCE_RANK_COLUMN_SPECIAL 10
#define FORCE_RANK_COLUMN_NONE    11   /* holds skill id 0 only */
#define FORCE_RANK_COLUMN_COUNT   12

/* One column of the skills screen: the skill ids listed in it. */
typedef struct ForceRankColumn {
    u8 skills[6];
    u8 count;                          /* 0: the cursor skips the column */
} ForceRankColumn;

/* Work area of the Force Rank selection screen, through gForceRankScreen. */
typedef struct ForceRankScreen {
    ForceRankColumn columns[FORCE_RANK_COLUMN_COUNT];
    u8 pad_54[0x7a - 0x54];
    u8 unk_7a;                         /* columns up to this one are listed; L and R cycle unk_7b through 0 to min(4, unk_7a) - 1 */
    u8 unk_7b;
    u8 pad_7c;
    u8 row;                            /* row the cursor keeps when it changes column */
} ForceRankScreen;

extern ForceRankScreen *gForceRankScreen;
extern ForceRankInfo Ov000_gForceRankTable[128];

#endif
