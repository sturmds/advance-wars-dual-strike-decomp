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
    u8 unk_1c_0 : 1;
    u8 unk_1c_1 : 1;
    u8 unk_1c_2 : 6;
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

/* Five rows of 31 cells (62 bytes) at the start of Ov003State. The cell at
 * index 29 holds the number of spawn records the row uses (read as a plain byte). */
typedef struct Ov003Row {
    Ov003Cell cells[29];
    u8 spawnCount;
    u8 pad_3b[3];
} Ov003Row;

typedef struct Ov003Slot {
    u8 pad_00;
    u8 row : 4;                        /* 1 + row of cells; bit 0 picks Ov003Unit.unk_12_0 */
    u8 pad_02[2];
} Ov003Slot;

/* 5 bytes at Ov003State.spawns: where a unit appears and how it starts. */
typedef struct Ov003Spawn {
    u8 x;                              /* passed to func_ov003_0235c42c */
    u8 y;
    u8 unk_2_0 : 3;                    /* becomes Ov003Unit.unk_12_0 */
    u8 unk_2_3 : 3;                    /* bit 0 becomes Ov003Unit.unk_1e_0 */
    u8 unk_2_6 : 1;                    /* becomes Ov003Unit.unk_1d_7 */
    u8 unk_2_7 : 1;                    /* becomes Ov003Unit.unk_1d_6 */
    u8 unk_3;                          /* becomes Ov003Unit.unk_1f */
    u8 unk_4;                          /* becomes Ov003Unit.unk_1e_1 */
} Ov003Spawn;

/* Overlay 3 state, through data_ov003_02364928. */
typedef struct Ov003State {
    Ov003Row rows[5];
    u8 pad_136[0x138 - 0x136];
    Ov003Slot slots[7];                /* length unknown */
    u8 pad_154[0x156 - 0x154];
    Ov003Spawn spawns[1];              /* length unknown */
    u8 pad_15b[0x1c8 - 0x15b];
    u32 frameCount;                    /* frames on which func_ov003_02362258 returns 1 */
    u32 unk_1cc;
    u16 unk_1d0;
    u16 unk_1d2;
    u8 pad_1d4;
    u8 currentSlot;
    u16 unk_1d6;
    u8 pad_1d8[0x1da - 0x1d8];
    u8 unk_1da;
    u8 row : 7;                        /* 1 to 5: the row spawns go to */
    u8 unk_1db_7 : 1;
    u8 pad_1dc;
    u8 nextSpawn;                      /* index of the next record in spawns and cell in the row */
    u8 pad_1de;
    u8 unk_1df_0 : 1;
    u8 unk_1df_1 : 1;
    u8 unk_1df_2 : 3;
    u8 unk_1df_5 : 2;
    u8 pauseCursor : 1;                /* pause menu: 0 top button, 1 bottom button */
} Ov003State;

/* Map of overlay 3, through data_ov003_02364a88. */
#define OV003_MAP_TILE_MAX 0x508

typedef struct Ov003Map {
    u8 width;
    u8 height;
    u16 cells[OV003_MAP_TILE_MAX];     /* tile code per tile, row by row */
    u8 marks[OV003_MAP_TILE_MAX];      /* per tile: MARK_* bits (Ov003_MarkRowTiles) */
} Ov003Map;

/* This frame's touch and key input, copied by Ov003_ReadInput into
 * data_ov003_02365b04. */
typedef struct Ov003Input {
    u8 phase;                          /* TouchState.phase */
    u8 active;                         /* low byte of TouchState.active */
    u8 x;
    u8 y;
    u16 keys;                          /* Keypad.unk_00 */
} Ov003Input;

extern Ov003State *data_ov003_02364928;
extern Ov003Map *data_ov003_02364a88;

#endif
