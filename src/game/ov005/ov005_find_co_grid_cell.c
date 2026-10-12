#include "types.h"
#include "game/Unk_ov005_0236bba8.h"

/* Returns the index of the grid cell at (col, row), or OV005_CO_GRID_ENTRIES when there is none. */
int Ov005_FindCoGridCell(Unk_ov005_0236bba8* work, int col, int row) {
  Ov005CoGridEntry* cell = work->grid;
  int i;

  for (i = 0; i < OV005_CO_GRID_ENTRIES; i++, cell++) {
    if (cell->column == col && cell->row == row) {
      return i;
    }
  }
  return i;
}
