#ifndef GAME_UNK_022A7FAC_H
#define GAME_UNK_022A7FAC_H

#include "types.h"

/* Main bss state at data_022a7fac: twelve signed bytes, cleared together
 * by MainMenu_Prepare and read one by one by small getters. */
typedef struct Unk_022a7fac {
    u8 pad_00[4];
    s8 unk_04;
    s8 unk_05;
    s8 unk_06;
    u8 pad_07[5];
} Unk_022a7fac;

#endif
