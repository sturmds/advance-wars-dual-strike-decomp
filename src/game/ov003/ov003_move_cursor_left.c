#include "types.h"
#include "game/ov003_unit.h"

void Sound_PlaySe(int se);
int Ov003_MoveCursorRight(int slot);

/* D-pad left while placing units. The cells of a row form two columns of 10
 * (cell n and n + 10 sit side by side). From the right column, moves the
 * cursor to the free cell (state 1) in the left column: the same line first,
 * then upwards, then downwards from the next one. Plays a sound for the
 * current slot. Returns 1 if the cursor moved. From the left column it
 * hands over to Ov003_MoveCursorRight, which wraps around. */
int Ov003_MoveCursorLeft(int slot) {
  Ov003Row* row;
  u32 cursor;
  int rowNumber;
  int i;
  Ov003Cell* cell;

  rowNumber = data_ov003_02364928->slots[slot].row;
  cursor = data_ov003_02364928->slots[slot].cursor;
  if (cursor < 10) {
    return Ov003_MoveCursorRight(slot);
  }
  row = &data_ov003_02364928->rows[rowNumber - 1];
  cell = &row->cells[cursor] - 10;
  for (i = cursor - 10; i >= 0; i--, cell--) {
    if (cell->state == 1) {
      if (slot == data_ov003_02364928->currentSlot) {
        Sound_PlaySe(0x10002);
      }
      data_ov003_02364928->slots[slot].cursor = i;
      return 1;
    }
  }
  cell = &row->cells[cursor] - 9;
  for (i = cursor - 9; i < 10; i++, cell++) {
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
