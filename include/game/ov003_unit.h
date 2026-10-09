#ifndef GAME_OV003_UNIT_H
#define GAME_OV003_UNIT_H

#include "types.h"

/* One of the 16 32-byte entries in data_ov003_02369538 (overlay 3). A new
 * entry starts with hp 100; a free entry has hp 0. */
typedef struct Ov003Unit {
    s16 x;                             /* 128 per tile, plus 64 to centre it */
    s16 y;
    u16 unk_04;
    u8 pad_06[2];
    s16 unk_08;
    s16 startX;                        /* copy of x when created */
    s16 unk_0c;                        /* copy of x too, not of y */
    u16 startUnk_04;                   /* copy of unk_04 */
    u8 startHp;                        /* copy of hp */
    u8 unk_11;
    u8 unk_12_0 : 3;                   /* 4 for an odd Ov003Cell nibble, else 3 */
    u8 unk_12_3 : 5;                   /* nibble & 31 */
    u8 tile;                           /* tile code of the cell it came from */
    u8 hp;                             /* 100 = full; 0 = free entry */
    u8 unk_15;
    u8 unk_16;
    u8 unk_17_0 : 6;
    u8 unk_17_6 : 1;
    u8 unk_17_7 : 1;
    u8 unk_18;
    u8 unk_19;
    s8 unk_1a;
    s8 unk_1b;
    u8 pad_1c;
    u8 unk_1d_0 : 6;
    u8 unk_1d_6 : 1;
    u8 unk_1d_7 : 1;
    u8 unk_1e_0 : 1;
    u8 unk_1e_1 : 7;
    u8 unk_1f;
} Ov003Unit;

typedef char Ov003Unit_size_check[sizeof(Ov003Unit) == 0x20 ? 1 : -1];

/* 2-byte cell of a 31-cell row in data_ov003_02364928 (62 bytes per row). */
typedef struct Ov003Cell {
    u8 tile : 6;
    u8 state : 2;                      /* 2: an Ov003Unit sits here */
    u8 unit;                           /* index in data_ov003_02369538 */
} Ov003Cell;

#endif
