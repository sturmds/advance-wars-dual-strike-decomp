#include "types.h"
#include "game/ov003_unit.h"

void Sound_PlaySe(int se);
int Ov003_MoveCursorLeft(int slot);

/* D-pad right while placing units: same as Ov003_MoveCursorLeft in the other direction. From
 * the left column, moves the cursor to a free cell in the right column (cells
 * 10 and up, stopping at the row's spawnCount). From the right column it hands
 * over to Ov003_MoveCursorLeft. Returns 1 if the cursor moved. */
int Ov003_MoveCursorRight(int slot) {
  Ov003Row* row;
  u32 cursor;
  int rowNumber;
  int i;
  Ov003Cell* cell;

  rowNumber = data_ov003_02364928->slots[slot].row;
  cursor = data_ov003_02364928->slots[slot].cursor;
  if (cursor >= OV003_COLUMN_CELL_COUNT) {
    return Ov003_MoveCursorLeft(slot);
  }
  row = &data_ov003_02364928->rows[rowNumber - 1];
  cell = &row->cells[cursor] + OV003_COLUMN_CELL_COUNT;
  for (i = cursor + OV003_COLUMN_CELL_COUNT;
       i < data_ov003_02364928->rows[rowNumber - 1].spawnCount; i++, cell++) {
    if (cell->state == 1) {
      if (slot == data_ov003_02364928->currentSlot) {
        Sound_PlaySe(0x10002);
      }
      data_ov003_02364928->slots[slot].cursor = i;
      return 1;
    }
  }
  cell = &row->cells[cursor] + (OV003_COLUMN_CELL_COUNT - 1);
  for (i = cursor + (OV003_COLUMN_CELL_COUNT - 1); i >= OV003_COLUMN_CELL_COUNT;
       i--, cell--) {
    if (cell->state == 1) {
      if (slot == data_ov003_02364928->currentSlot) {
        Sound_PlaySe(0x10002);
      }
      data_ov003_02364928->slots[slot].cursor = i;
      return 1;
    }
  }
  return 0;
}
