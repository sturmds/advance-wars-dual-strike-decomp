#ifndef GAME_UNK_02160AC0_H
#define GAME_UNK_02160AC0_H

#include "types.h"

/* 28 bytes per row at the start of Unk_02160ac0 (4 rows): the cell bytes of
 * an Ov003Row and its spawn count at offset 0x18. */
typedef struct Unk_02160ac0_Row {
    struct {
        u8 tile : 6;
        u8 state : 2;
    } cells[24];
    u8 spawnCount;
    u8 pad_19[3];
} Unk_02160ac0_Row;

/* Pointed to by data_02160ac0 (main data). func_0208bf40 fills bytes
 * 0x00-0x13, 0x18, 0x19, 0x7d and 0x84 from data_0229e148. */
typedef struct Unk_02160ac0 {
    struct {
        u8 tile : 6;
        u8 state : 2;
    } cells[20];
    u32 unk_14;                        /* Combat mode funds: the four entries of CombatModeMenu_DrawFundsAndPrices cost 1000 to 6000 */
    u8 unk_18;                         /* cells in use, at most 20 */
    u8 unk_19;
    u8 pad_1a[0x70 - 0x1a];
    u8 unk_70[8];                      /* Ov003Slot.row per slot (overlay 3, link play) */
    u8 pad_78[0x7c - 0x78];
    u8 unk_7c;                         /* nonzero: Ov003_InitState uses a fixed random seed */
    u8 unk_7d;
    u8 unk_7e;
    u8 unk_7f;                         /* 0xff: overlay 3 uses its preset's value */
    u8 unk_80;
    u8 unk_81;                         /* 0xff: overlay 3 uses its preset's value */
    u8 unk_82;                         /* 0xff: overlay 3 uses its preset's value */
    u8 unk_83;                         /* 0-4: picks the help text of entry 4 in LinkMenu_DrawHelpText */
    u8 unk_84;
} Unk_02160ac0;

extern Unk_02160ac0 *data_02160ac0;

#endif
