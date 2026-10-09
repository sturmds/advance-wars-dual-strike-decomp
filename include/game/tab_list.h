#ifndef GAME_TAB_LIST_H
#define GAME_TAB_LIST_H

#include "types.h"

/* Start of gMenuState on the screen that TabList_RebuildButtons lays out: a list of
 * entries in tabs, nine per page. */
typedef struct TabListView {
    u8 pageOfTab[4];                   /* first page shown, per tab */
    u8 tab;
    u8 row;                            /* row of the cursor on the last page, set by TabList_RebuildButtons */
} TabListView;

/* One entry of data_0215d6c4, 0x30 bytes: 44 entries per row, one row per
 * tab of the screen that TabList_RebuildButtons lays out. */
typedef struct TabListEntry {
    u32 textId;
    u16 unk_04;                        /* 0x68 marks an unused entry */
    u8 pad_06[0x14 - 0x06];
    int (*getValue)(int row, int col); /* the number drawn after the text */
    u8 pad_18[0x30 - 0x18];
} TabListEntry;

extern TabListEntry data_0215d6c4[][44];

#endif
