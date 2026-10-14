#include "types.h"
#include "game/ov003_unit.h"

void Ov003Rows_ClearCells(void) {
  int rowNumber;
  int i;
  Ov003Cell* cell;

  for (rowNumber = 1; rowNumber <= OV003_ROW_COUNT; rowNumber++) {
    data_ov003_02364928->rows[rowNumber - 1].spawnCount = 0;
    cell = data_ov003_02364928->rows[rowNumber - 1].cells;
    for (i = 0; i < OV003_ROW_CELL_COUNT; i++, cell++) {
      cell->state = 0;
      *(s8*)&cell->unit = -1; /* needed to match: Ov003Cell.unit as s8 */
    }
  }
}
