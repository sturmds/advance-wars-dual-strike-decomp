#include "types.h"
#include "game/ov003_unit.h"

extern u16 data_ov003_02364cdc[][2];
int Ov003_IsRowPowerActive(int rowNumber);

/* Adds amount to the row's charge, up to the limit of its CO. Does nothing unless unk_1df_6 is set, while the power runs,
 * or for row 5. The row expression is repeated on purpose (needed to match). */
void Ov003Row_AddCharge(int rowNumber, int amount) {
  if (!data_ov003_02364928->unk_1df_6) {
    return;
  }
  if (Ov003_IsRowPowerActive(rowNumber) == 1) {
    return;
  }
  if (rowNumber == 5) {
    return;
  }
  data_ov003_02364928->rows[rowNumber - 1].charge += amount;
  if (data_ov003_02364928->rows[rowNumber - 1].charge >
      data_ov003_02364cdc[data_ov003_02364928->rows[rowNumber - 1].co][0]) {
    data_ov003_02364928->rows[rowNumber - 1].charge =
        data_ov003_02364cdc[data_ov003_02364928->rows[rowNumber - 1].co][0];
  }
}
