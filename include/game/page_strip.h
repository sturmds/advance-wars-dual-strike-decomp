#ifndef GAME_PAGE_STRIP_H
#define GAME_PAGE_STRIP_H

#include "types.h"

/* One list of buttons for PageStrip_AddButtons: the selected entry
 * and the entry count. */
typedef struct PageStripList {
  u8 cursor;
  u8 count;
  u8 unk_02;
  u8 unk_03;
} PageStripList;

/* Allocated work area, through data_0229c744. One list for each value of
 * SurvivalSelect_GetList. */
typedef struct PageStrip {
  PageStripList lists[2];
  u16 colours[16]; /* palette cycle, one colour every 4 frames */
} PageStrip;

extern PageStrip* data_0229c744;

/* Task that draws a page strip each frame (PageStripTask_Draw). PageStripTask_Create
 * makes it and copies these fields from the task that created it. The first
 * 0x58 bytes belong to the task system (Task_Create). */
typedef struct PageStripTask {
  u8 pad_00[0x58];
  int count; /* entries in the strip */
  int pages;
  u8 pad_60[0x64 - 0x60];
  s16 cursor; /* selected entry */
  s16 unk_66; /* with 2 or more pages, entries before unk_66 mod count use the second palette */
  u16 mapId;  /* map whose player count is shown */
} PageStripTask;

#endif
