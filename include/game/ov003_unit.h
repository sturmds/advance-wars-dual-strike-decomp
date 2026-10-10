#ifndef GAME_OV003_UNIT_H
#define GAME_OV003_UNIT_H

#include "types.h"

/* One of the 16 32-byte entries in data_ov003_02369538 (overlay 3). A new
 * entry starts with hp 100; a free entry has hp 0. */
typedef struct Ov003Unit {
    s16 x;                             /* 128 per tile, plus 64 to centre it */
    s16 y;
    u16 unk_04;
    u16 lastTile;                      /* index of the map tile checked last (Ov003_UpdateTileClaims) */
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

/* 20-byte entry of data_ov003_02364d88, indexed by Ov003Unit.tile. */
typedef struct Ov003TileInfo {
    u16 unk_00;                        /* divisor for Ov003Unit.unk_04 in Ov003_DrawUnitAnims */
    u8 pad_02[0x14 - 0x02];
} Ov003TileInfo;

/* 2-byte cell of a 31-cell row in data_ov003_02364928 (62 bytes per row). */
typedef struct Ov003Cell {
    u8 tile : 6;
    u8 state : 2;                      /* 2: an Ov003Unit sits here */
    u8 unit;                           /* index in data_ov003_02369538 */
} Ov003Cell;

/* Five 62-byte rows at the start of Ov003State: 20 cells, then the row's
 * settings. Ov003_InitRows clears 0x28-0x39. */
typedef struct Ov003Row {
    Ov003Cell cells[20];
    u8 unk_28[10];
    u16 unk_32;
    u16 unk_34;
    u8 pad_36[2];
    u8 unk_38;
    u8 unk_39;
    u8 spawnCount;
    u8 unk_3b_0 : 4;                   /* 1-3; 8-10 are resolved by Ov003_InitRows */
    u8 unk_3b_4 : 4;                   /* even number per active row, then shifted to centre them */
    u8 color : 7;                      /* 1-based: picks this row's palette colours and tiles */
    u8 active : 1;
    u8 pad_3d;
} Ov003Row;

typedef struct Ov003Slot {
    u8 unk_00;
    u8 row : 4;                        /* 1 + row of cells; bit 0 picks Ov003Unit.unk_12_0 */
    u8 unk_01_4 : 4;                   /* func_ov003_023572b0 tests it for 2 */
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

/* 20-byte entry of data_ov003_02364e50, picked by data_02160ac0->unk_7d:
 * how Ov003_InitRows sets up the rows. */
typedef struct Ov003Preset {
    u8 pad_00[4];
    u32 unk_04;                        /* passed to func_ov003_0235bcc8 */
    u8 colors[4];                      /* Ov003Row.color per row */
    u8 unk_0c[4];                      /* Ov003Row.unk_3b_0 per row */
    u8 rowCount : 3;
    u8 unk_10_3 : 1;                   /* default for Ov003State.unk_1df_0 */
    u8 unk_10_4 : 4;                   /* default for Ov003State.unk_1df_2 */
    u8 unk_11;                         /* default for Ov003State.unk_1dc */
    u8 timeLimitMinutes;               /* 0: no time limit (Ov003_DrawTimeLeft) */
    u8 pad_13;
} Ov003Preset;

/* Overlay 3 state, through data_ov003_02364928. */
typedef struct Ov003State {
    Ov003Row rows[5];
    u8 pad_136[0x138 - 0x136];
    Ov003Slot slots[7];                /* length unknown */
    u8 pad_154[0x156 - 0x154];
    Ov003Spawn spawns[1];              /* length unknown */
    u8 pad_15b[0x1bc - 0x15b];
    Ov003Preset *preset;
    u8 unk_1c0[8];
    u32 frameCount;                    /* frames on which func_ov003_02362258 returns 1 */
    u32 unk_1cc;
    union {
        u16 unk_1d0;
        s16 scrollX;                   /* map scroll in pixels */
    };
    union {
        u16 unk_1d2;
        s16 scrollY;
    };
    u8 activeRows;                     /* bit per row with Ov003Row.active set */
    u8 currentSlot;
    u16 unk_1d6;
    u8 rowCount;                       /* rows in use */
    u8 unk_1d9;
    union {
        u8 unk_1da;
        struct {
            u8 unk_1da_0 : 4;          /* 1: the row of the current slot (or Ov003State.row) is picked */
            u8 unk_1da_4 : 4;
        };
    };
    u8 row : 7;                        /* 1 to 5: the row spawns go to */
    u8 unk_1db_7 : 1;
    u8 unk_1dc;
    u8 nextSpawn;                      /* index of the next record in spawns and cell in the row */
    u8 pad_1de;
    u8 unk_1df_0 : 1;
    u8 unk_1df_1 : 1;
    u8 unk_1df_2 : 3;
    u8 unk_1df_5 : 1;
    u8 unk_1df_6 : 1;
    u8 pauseCursor : 1;                /* pause menu: 0 top button, 1 bottom button */
} Ov003State;

/* View of Ov003State.unk_1da as two nibbles: func_ov003_02362258 switches on
 * the low one (0 to 6); Ov003_AdvanceMode copies the high one into it. */
typedef struct Ov003NibblePair {
    u8 low : 4;
    u8 high : 4;
} Ov003NibblePair;

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

/* Camera position in pixels: Ov003State.unk_1d0 and unk_1d2, read as s16. */
#define OV003_SCROLL_X ((s16)data_ov003_02364928->unk_1d0)
#define OV003_SCROLL_Y ((s16)data_ov003_02364928->unk_1d2)

#endif
