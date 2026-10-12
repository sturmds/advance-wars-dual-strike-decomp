#include "types.h"
#include "game/Unk_02080440.h"
#include "game/design_map_edit.h"

extern Unk_02080440* data_027c027c;
extern u32 data_027c0284;

/* data_027c027c->unk_48[data_027c0284], written so that the compiler loads
 * the index first, as the original does. */
#define STATE                                                             \
  (*(Unk_02080440_48**)((u8*)(data_027c0284 * sizeof(Unk_02080440_48*)) + \
                            (u32)data_027c027c + (u32) &                  \
                        ((Unk_02080440*)0)->unk_48))

#define CELL_AT(x, y) \
  (STATE->mapTiles->mapCells[(x) + STATE->mapTiles->rowStart[y]])
/* Straight pipe cells; the directions are a guess from the checks below. */
#define CELL_PIPE_HORIZONTAL 0x142 /* joins left and right */
#define CELL_PIPE_VERTICAL 0x143   /* joins up and down */
#define IS_SEAM_CELL(cell) \
  ((cell) == MAP_CELL_PIPE_SEAM_H || (cell) == MAP_CELL_PIPE_SEAM_V)

/* The Design Room calls this before it puts a pipe seam (terrain 16) on
 * x, y; TRUE refuses it. A seam goes only on a straight pipe cell, and not
 * next to another seam along the pipe. */
BOOL Ov006_CannotPlacePipeSeam(int x, int y) {
  u16 cell = CELL_AT(x, y);

  if (cell == CELL_PIPE_HORIZONTAL) {
    if (x - 1 >= 0 && IS_SEAM_CELL(CELL_AT(x - 1, y))) {
      return TRUE;
    }
    if (x + 1 < STATE->unk_1c4->mapWidth && IS_SEAM_CELL(CELL_AT(x + 1, y))) {
      return TRUE;
    }
  } else if (cell == CELL_PIPE_VERTICAL) {
    if (y - 1 >= 0 && IS_SEAM_CELL(CELL_AT(x, y - 1))) {
      return TRUE;
    }
    if (y + 1 < STATE->unk_1c4->mapHeight && IS_SEAM_CELL(CELL_AT(x, y + 1))) {
      return TRUE;
    }
  } else {
    return TRUE;
  }
  return FALSE;
}
