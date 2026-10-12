#include "types.h"
#include "game/ov003_unit.h"

/* row is 1 to 5. */
int Ov003_IsRowPowerActive(int row) {
  return data_ov003_02364928->rows[row - 1].powerTimer != 0;
}
