#include "types.h"
#include "game/ov003_unit.h"

void Ov003Row_AppendCell(int rowNumber, u8 tile, u8 state) {
  Ov003Row* row = &data_ov003_02364928->rows[rowNumber - 1];
  u8 count = row->spawnCount;

  if (count >= 20) {
    return;
  }
  row->cells[count].kind = tile;
  row->cells[count].state = state;
  data_ov003_02364928->rows[rowNumber - 1].spawnCount++;
}
