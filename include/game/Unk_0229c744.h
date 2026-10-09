#ifndef GAME_UNK_0229C744_H
#define GAME_UNK_0229C744_H

#include "types.h"

/* One list of buttons for PageStrip_AddButtons: the selected entry
 * and the entry count. */
typedef struct Unk_0229c744_List {
    u8 cursor;
    u8 count;
    u8 unk_02;
    u8 unk_03;
} Unk_0229c744_List;

/* Allocated work area, through data_0229c744. One list for each value of
 * func_0204779c. */
typedef struct Unk_0229c744 {
    Unk_0229c744_List lists[2];
} Unk_0229c744;

extern Unk_0229c744 *data_0229c744;

#endif
