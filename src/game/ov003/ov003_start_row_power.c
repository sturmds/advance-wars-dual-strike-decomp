#include "types.h"
#include "game/ov003_unit.h"

extern u16 data_ov003_02364cde[][2];

void Ov003_HealRowUnits(int row);
void func_ov003_0235db04(int row);
void Ov003Row_StopPower(int row);

/* Starts the row's power: sets the timer from the table, runs the start effect
 * of kind 1 (heal) and 7 (task), and calls Ov003Row_StopPower at once when
 * the timer is 0. */
void Ov003_StartRowPower(int row) {
  data_ov003_02364928->rows[row - 1].powerTimer =
      data_ov003_02364cde[data_ov003_02364928->rows[row - 1].co][0];
  switch (data_ov003_02364928->rows[row - 1].co) {
    case 1:
      Ov003_HealRowUnits(row);
      break;
    case 7:
      func_ov003_0235db04(row);
      break;
  }
  if (data_ov003_02364928->rows[row - 1].powerTimer == 0) {
    Ov003Row_StopPower(row);
  }
}
