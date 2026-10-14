#ifndef GAME_UNK_02160AC0_H
#define GAME_UNK_02160AC0_H

#include "types.h"

/* 28 bytes per row at the start of Unk_02160ac0 (4 rows): the cell bytes of
 * an Ov003Row and its spawn count at offset 0x18. */
typedef struct Unk_02160ac0_Row {
  union {
    struct {
      u8 tile : 6;
      u8 state : 2;
    } cells[24];
    struct {
      u8 pad_00[0x14];
      u32 funds; /* a guess: CombatSetup_DrawShop compares it with the unit prices */
    };
  };
  u8 spawnCount;
  u8 unk_19; /* index into data_02160ae0, which gives the row's CO */
  u8 pad_1a[2];
} Unk_02160ac0_Row;

/* Row i (0-3) of data_02160ac0. */
#define LINK_ROWS ((Unk_02160ac0_Row*)data_02160ac0)

/* Pointed to by data_02160ac0 (main data). func_0208bf40 fills bytes
 * 0x00-0x13, 0x18, 0x19, 0x7d and 0x84 from data_0229e148. */
typedef struct Unk_02160ac0 {
  struct {
    u8 tile : 6;
    u8 state : 2;
  } cells[20];
  u32 funds; /* Combat mode funds: the four entries of CombatModeMenu_DrawFundsAndPrices cost 1000 to 6000 */
  u8 cellCount; /* cells in use, at most 20 */
  u8 co; /* CO slot (gCombatModeCos) in the menu; CombatModeMenu_Close turns it into a CombatCo number */
  u8 pad_1a[0x70 - 0x1a];
  u8 unk_70[8]; /* Ov003Slot.row per slot (overlay 3, link play) */
  u32 unk_78;   /* link menu entry 1: 10000-95000 in steps of 5000 */
  u8 unk_7c;    /* nonzero: Ov003_InitState uses a fixed random seed */
  u8 unk_7d;
  u8 unk_7e;
  u8 unk_7f; /* 0xff: overlay 3 uses its preset's value */
  u8 unk_80;
  u8 unk_81; /* 0xff: overlay 3 uses its preset's value */
  u8 unk_82; /* 0xff: overlay 3 uses its preset's value */
  u8 unk_83; /* 0-4: picks the help text of entry 4 in LinkMenu_DrawHelpText */
  u8 unk_84;
} Unk_02160ac0;

extern Unk_02160ac0 data_0229e0bc; /* what data_02160ac0 points to */
extern Unk_02160ac0* data_02160ac0;

/* Four counts per menu entry (8 to 11), one per row of icons. */
typedef struct IconCounts {
  u8 n[4 * 4];
} IconCounts;

/* data_02160afc, read by CombatSetup_DrawRowSprites. */
typedef struct UnitCountTable {
  u8 counts[4 * 4]; /* 4 per kind; needed to match: indexed as one array */
} UnitCountTable;

#endif
