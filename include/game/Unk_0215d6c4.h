#ifndef GAME_UNK_0215D6C4_H
#define GAME_UNK_0215D6C4_H

#include "types.h"

/* One entry of data_0215d6c4, 0x30 bytes: 44 entries per row, one row per
 * tab of the screen that TabList_RebuildButtons lays out. */
typedef struct Unk_0215d6c4 {
    u32 textId;
    u16 unk_04;                        /* 0x68 marks an unused entry */
    u8 pad_06[0x30 - 0x06];
} Unk_0215d6c4;

#endif
