#include "types.h"
#include "game/ov003_unit.h"

extern u8 data_ov003_02364cfc[];
int Ov003_IsRowPowerActive(int rowNumber);

/* Speed of a unit kind on a terrain, from a table of 23 terrains per kind
 * (terrain masked to 5 bits, out of range gives terrain 0). 0 means the unit
 * cannot enter. For a row: x1.5 while the power of CO 6 runs, halved while
 * slowTimer runs. Row 0 gets the table value as is. */
u8 Ov003_GetMoveSpeed(int rowNumber, int kind, int terrain) {
  u8 value;

  terrain &= 31;
  if (terrain >= 23) {
    terrain = 0;
  }
  value = data_ov003_02364cfc[kind * 23 + terrain];
  if (rowNumber == 0) {
    return value;
  }
  if (Ov003_IsRowPowerActive(rowNumber) == 1) {
    if (data_ov003_02364928->rows[rowNumber - 1].co == 6) {
      value = value * 3 >> 1;
    }
  }
  if (data_ov003_02364928->rows[rowNumber - 1].slowTimer != 0) {
    value = value >> 1;
  }
  return value;
}
