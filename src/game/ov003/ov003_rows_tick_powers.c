#include "types.h"
#include "game/ov003_unit.h"

void Ov003Row_StopPower(int rowNumber);

/* Counts down powerTimer of each active row among rows 1-4. */
void Ov003Rows_TickPowers(void) {
  int rowNumber;
  Ov003Row* row;

  for (rowNumber = 1; rowNumber <= 4; rowNumber++) {
    row = &data_ov003_02364928->rows[rowNumber - 1];
    if (row->active && row->powerTimer != 0) {
      row->powerTimer--;
      if (data_ov003_02364928->rows[rowNumber - 1].powerTimer == 0) {
        Ov003Row_StopPower(rowNumber);
      }
    }
  }
}
