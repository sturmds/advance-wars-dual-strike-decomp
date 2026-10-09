#ifndef GAME_UNK_02070250_H
#define GAME_UNK_02070250_H

#include "types.h"

/* Start of gMenuState on the screen that TabList_RebuildButtons lays out: a list of
 * entries in tabs, nine per page. */
typedef struct Unk_02070250 {
    u8 pageOfTab[4];                   /* first page shown, per tab */
    u8 tab;
    u8 row;                            /* row of the cursor on the last page, set by TabList_RebuildButtons */
} Unk_02070250;

#endif
