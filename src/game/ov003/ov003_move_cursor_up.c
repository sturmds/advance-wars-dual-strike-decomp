#include "types.h"
#include "game/ov003_unit.h"

void Sound_PlaySe(int se);

/* D-pad up while placing units: moves the slot's cursor to the previous free
 * cell (state 1) of its row, wrapping from the end of the row (spawnCount).
 * Plays a sound for the current slot. Returns 1 if the cursor moved. */
int Ov003_MoveCursorUp(int slot) {
  int rowIndex;
  int cursor;
  int i;
  Ov003Cell* cell;

  rowIndex = data_ov003_02364928->slots[slot].row - 1;
  cursor = data_ov003_02364928->slots[slot].cursor;
  cell = &data_ov003_02364928->rows[rowIndex].cells[cursor] - 1;
  for (i = cursor - 1; i >= 0; i--, cell--) {
    if (cell->state == 1) {
      if (slot == data_ov003_02364928->currentSlot) {
        Sound_PlaySe(0x10002);
      }
      data_ov003_02364928->slots[slot].cursor = i;
      return 1;
    }
  }
  cell = &data_ov003_02364928->rows[rowIndex]
              .cells[data_ov003_02364928->rows[rowIndex].spawnCount] -
         1;
  for (i = data_ov003_02364928->rows[rowIndex].spawnCount - 1;
       i > data_ov003_02364928->slots[slot].cursor; i--, cell--) {
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
