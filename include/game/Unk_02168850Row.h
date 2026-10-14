#ifndef GAME_UNK_02168850ROW_H
#define GAME_UNK_02168850ROW_H

#include "types.h"

/* One row of data_02168850: three file names in /bmap for the unit
 * highlight effects. func_020e3554, func_020e353c and func_020e3524 return
 * one each; the last two through symbols at +4 and +8 of the table. */
typedef struct Unk_02168850Row {
  const char* unk_00; /* tiles, by effect (PowerHighlight_LoadGraphics) */
  const char* unk_04;
  const char* unk_08; /* palette, by colour (PowerHighlight_LoadGraphics) */
} Unk_02168850Row;

typedef char
    Unk_02168850Row_size_check[sizeof(Unk_02168850Row) == 0xc ? 1 : -1];

extern Unk_02168850Row data_02168850[];

#endif
