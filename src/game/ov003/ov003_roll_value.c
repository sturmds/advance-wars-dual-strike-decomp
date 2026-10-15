#include "types.h"
#include "game/ov003_unit.h"
#include "game/math.h"

extern u8 data_ov003_02364d8f[][20];

u32 Ov003_RandNext(void);
int Ov003_IsRowPowerActive(int row);

/* Starts from data_ov003_02364d8f[kind][index] plus a random -3 to 4, then
 * scales it by the settings of the row that gets it (attackerRow: mode 2
 * gives 3/2, mode 4 gives 5/10 to 19/10 at random) and of the row that takes
 * it (defenderRow: mode 5 gives 2/3). Row 0 is no row. Rows that
 * Ov003_IsRowPowerActive does not accept are not scaled. What the values stand
 * for is not known. */
int Ov003_RollValue(int attackerRow, int kind, int defenderRow, int index) {
  u8 value;

  value = data_ov003_02364d8f[kind][index];
  value += (Ov003_RandNext() & 7) - 3;
  if (Ov003_IsRowPowerActive(attackerRow) == 1) {
    switch (data_ov003_02364928->rows[attackerRow - 1].co) {
      case 2:
        value = (value * 3) >> 1;
        break;
      case 4:
        value = HwDiv32(value * (HwMod32(Ov003_RandNext(), 15) + 5), 10);
        break;
    }
  }
  if (defenderRow == 0) {
    return value;
  }
  if (Ov003_IsRowPowerActive(defenderRow) == 1 &&
      data_ov003_02364928->rows[defenderRow - 1].co == 5) {
    value = HwDiv32(value * 2, 3);
  }
  return value;
}
