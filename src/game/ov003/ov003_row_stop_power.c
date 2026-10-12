#include "types.h"
#include "game/ov003_unit.h"

/* Ends the row's CO power: clears powerTimer and charge. */
void Ov003Row_StopPower(int rowNumber) {
  data_ov003_02364928->rows[rowNumber - 1].powerTimer = 0;
  data_ov003_02364928->rows[rowNumber - 1].charge = 0;
}
