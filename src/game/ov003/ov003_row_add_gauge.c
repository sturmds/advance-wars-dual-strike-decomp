#include "types.h"
#include "game/ov003_unit.h"

extern u16 data_ov003_02364cdc[][2];
int func_ov003_0235da18(int rowNumber);

/* Adds amount to the row's unk_32 (a gauge), up to the limit of its phase
 * (unk_3b_0). Does nothing unless unk_1df_6 is set, while the countdown runs,
 * or for row 5. The row expression is repeated on purpose (needed to match). */
void Ov003Row_AddGauge(int rowNumber, int amount)
{
    if (!data_ov003_02364928->unk_1df_6) {
        return;
    }
    if (func_ov003_0235da18(rowNumber) == 1) {
        return;
    }
    if (rowNumber == 5) {
        return;
    }
    data_ov003_02364928->rows[rowNumber - 1].unk_32 += amount;
    if (data_ov003_02364928->rows[rowNumber - 1].unk_32
        > data_ov003_02364cdc[data_ov003_02364928->rows[rowNumber - 1].unk_3b_0][0]) {
        data_ov003_02364928->rows[rowNumber - 1].unk_32
            = data_ov003_02364cdc[data_ov003_02364928->rows[rowNumber - 1].unk_3b_0][0];
    }
}
