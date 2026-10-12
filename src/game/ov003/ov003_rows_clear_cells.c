#include "types.h"
#include "game/ov003_unit.h"

void Ov003Rows_ClearCells(void) {
  int rowNumber;
  int i;
  Ov003Cell* cell;

  for (rowNumber = 1; rowNumber <= 5; rowNumber++) {
    data_ov003_02364928->rows[rowNumber - 1].spawnCount = 0;
    cell = data_ov003_02364928->rows[rowNumber - 1].cells;
    for (i = 0; i < 20; i++, cell++) {
      cell->state = 0;
      *(s8*)&cell->unit = -1; /* needed to match: Ov003Cell.unit as s8 */
    }
  }
}
