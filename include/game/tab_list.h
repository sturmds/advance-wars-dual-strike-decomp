#ifndef GAME_TAB_LIST_H
#define GAME_TAB_LIST_H

#include "types.h"

/* Start of gMenuState on the screen that TabList_RebuildButtons lays out: a list of
 * entries in tabs, nine per page. */
typedef struct TabListView {
  u8 pageOfTab[4]; /* first page shown, per tab */
  u8 tab;
  u8 row; /* row of the cursor on the last page, set by TabList_RebuildButtons */
  u8 unk_06; /* cleared with the others by HistoryScreen_Init */
  u8 pad_07[0x1000 - 0x07];
  u16 unk_1000
      [16]; /* a palette copied from res_history_medal by HistoryScreen_Init */
} TabListView;

/* TabListEntry.unk_04 value of the entry after the last one in a tab. */
#define TAB_LIST_END_MARK 0x68

/* One entry of data_0215d6c4, 0x30 bytes: 44 entries per row, one row per
 * tab of the screen that TabList_RebuildButtons lays out. */
typedef struct TabListEntry {
  u32 textId;
  u16 unk_04; /* 0x68 marks an unused entry */
  u8 pad_06[0x08 - 0x06];
  u32 medalThreshold[3]; /* getValue must reach threshold [n] for medal n + 1 */
  int (*getValue)(int row, int col); /* the number drawn after the text */
  void (*draw)(
      int x, int y, int row,
      int col); /* draws the entry at tile column x, row y (TabList_DrawPage) */
  int (*unk_1c)(int row,
                int col); /* may be NULL; TabList_CallUnk1c returns 0 then */
  u32 helpTextId[4]; /* help text of each column: 0 the entry, 1-3 the medals */
} TabListEntry;

extern TabListEntry data_0215d6c4[][44];

#endif
