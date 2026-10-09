#ifndef GAME_FORCE_RANK_SCREEN_H
#define GAME_FORCE_RANK_SCREEN_H

#include "types.h"

/* The Force Rank selection screen lays its entries out in 12 columns
 * (0-11) of up to 6 rows. A cursor holds the column in bits 3 and up and
 * the row in bits 0-2. */
#define FORCE_RANK_COLUMNS 12
#define FORCE_RANK_LAST_COLUMN 11

typedef struct ForceRankColumn {
    u8 entries[6];
    u8 count;                          /* 0: the cursor skips the column */
} ForceRankColumn;

/* Work area of the Force Rank selection screen, through data_0229c738. */
typedef struct ForceRankScreen {
    ForceRankColumn columns[FORCE_RANK_COLUMNS];
    u8 pad_54[0x7a - 0x54];
    u8 unk_7a;                         /* L and R cycle unk_7b through 0 to min(4, unk_7a) - 1 */
    u8 unk_7b;
    u8 pad_7c;
    u8 row;                            /* row the cursor keeps when it changes column */
} ForceRankScreen;

extern ForceRankScreen *data_0229c738;

#endif
