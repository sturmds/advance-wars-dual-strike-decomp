#ifndef GAME_UNK_022A7A9C_H
#define GAME_UNK_022A7A9C_H

#include "types.h"

/* data_022a7a9c (main bss). ScoreList_AddRow reads these bytes as points
 * (0x00-0x02) or as counts it multiplies into points (0x08-0x0d). */
typedef struct Unk_022a7a9c {
    u8 unk_00;
    u8 unk_01;
    u8 unk_02;
    u8 pad_03[0x08 - 0x03];
    u8 unk_08;
    u8 unk_09;
    u8 unk_0a;
    u8 unk_0b;
    u8 unk_0c;
    u8 unk_0d;
} Unk_022a7a9c;

extern Unk_022a7a9c data_022a7a9c;

#endif
