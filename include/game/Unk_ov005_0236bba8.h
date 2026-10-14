#ifndef GAME_UNK_OV005_0236BBA8_H
#define GAME_UNK_OV005_0236BBA8_H

#include "types.h"

/* CO picture screen work block (0x2f4 bytes) that func_ov005_02362618
 * allocates into Ov005Task.work and data_ov005_0236bba8. */
/* One palette variant button, an entry of Unk_ov005_0236bba8.paletteButtons:
 * 8 in row 0, one more in row 1. */
typedef struct CoPaletteButton {
  u8 pad_00[0x2];
  s16 x; /* pixels */
  s16 y;
  s16 col;
  s16 row;
  s16 buttonId; /* MenuButton id */
} CoPaletteButton;

#define CO_PALETTE_BUTTONS 9

struct Ov005Task;

/* One cell of the CO picture screen's grid, 16 bytes. The grid is 4
 * columns by 7 rows (data_ov005_0236ac90). */
typedef struct Ov005CoGridEntry {
  s16 index;
  s16 coId; /* -1: empty cell */
  s16 x;    /* pixels */
  s16 y;
  s16 column;
  s16 row;
  s16 available; /* 1 when CoSelect_IsCoAvailable accepts coId */
  s16 buttonId;  /* MenuButton id of the cell, 0x80 + index */
} Ov005CoGridEntry;

#define OV005_CO_GRID_COLUMNS 4
#define OV005_CO_GRID_ROWS 7
#define OV005_CO_GRID_ENTRIES (OV005_CO_GRID_COLUMNS * OV005_CO_GRID_ROWS)

typedef struct Unk_ov005_0236bba8 {
  u32 unk_00;
  u32 gridConfirmTouched; /* 1: acts as A on the grid (Ov005_OnCoGridButtonConfirm) */
  u32 paletteConfirmTouched; /* 1: acts as A on the palette buttons */
  u32 backTouched;           /* 1: acts as B */
  u8 pad_10[0x1c - 0x10];
  u32 paletteMode; /* 1 while the palette buttons are shown */
  u32 coId;        /* low 7 bits: CO shown */
  u32 cursorCoId;  /* coId of the grid cell at cursor */
  int unk_28;      /* palette variant of coId */
  int unk_2c;
  u32 cursor;     /* grid cell index */
  u32 prevCursor; /* cursor of the last frame; a change plays the move sound */
  u32 cursorRow;  /* row kept when the cursor moves left or right; -1: none */
  Ov005CoGridEntry
      grid[OV005_CO_GRID_ENTRIES]; /* copy of data_ov005_0236ac90 */
  fx32 unk_1fc;
  fx32 unk_200;
  fx32 unk_204;
  struct Ov005Task*
      childTask;   /* runs data_ov005_0236ab6c with the same work block */
  int variant;     /* palette variant of the CO */
  int prevVariant; /* variant of the last frame */
  int row0Variant; /* last variant picked in row 0 (0-7), where Up returns to from variant 8 */
  void* spriteFile; /* rikiishi file 042; source of the unk_21c sprites */
  void* unk_21c[9];
  int extraX[9]; /* palette button x + 13 (Ov005_InitCoPaletteButtons) */
  int extraY[9]; /* palette button y + 12 */
  CoPaletteButton
      paletteButtons[CO_PALETTE_BUTTONS]; /* copy of data_ov005_0236ac24 */
} Unk_ov005_0236bba8;

#endif
