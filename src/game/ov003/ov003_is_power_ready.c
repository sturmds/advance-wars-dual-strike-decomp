#include "types.h"
#include "game/ov003_unit.h"
#include "game/Unk_02160ac0.h"

extern u16 data_ov003_02364cdc[][2];

int Ov003_IsRowPowerActive(int row);

/* Returns 1 when the row's power can start: both phase nibbles agree, unk_1df_6
 * is set, no power is running, and the charge has reached the limit
 * for the row's kind. When data_02160ac0->unk_7c is set, Ov003State.row never
 * qualifies. */
int Ov003_IsPowerReady(int row) {
  if (data_ov003_02364928->phase != data_ov003_02364928->nextPhase) {
    return 0;
  }
  if (!data_ov003_02364928->unk_1df_6) {
    return 0;
  }
  if (data_02160ac0->unk_7c != 0 && row == data_ov003_02364928->row) {
    return 0;
  }
  if (Ov003_IsRowPowerActive(row) == 1) {
    return 0;
  }
  return data_ov003_02364928->rows[row - 1].charge >=
         data_ov003_02364cdc[data_ov003_02364928->rows[row - 1].co][0];
}
