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
 * func_0204779c. */
typedef struct PageStrip {
    PageStripList lists[2];
} PageStrip;

extern PageStrip *data_0229c744;

#endif
