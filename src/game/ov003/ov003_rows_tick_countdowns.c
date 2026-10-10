#include "types.h"
#include "game/ov003_unit.h"

void Ov003Row_StopCountdown(int rowNumber);

/* unk_34 of each active row among rows 1-4 is a countdown in frames. */
void Ov003Rows_TickCountdowns(void)
{
    int rowNumber;
    Ov003Row *row;

    for (rowNumber = 1; rowNumber <= 4; rowNumber++) {
        row = &data_ov003_02364928->rows[rowNumber - 1];
        if (row->active && row->unk_34 != 0) {
            row->unk_34--;
            if (data_ov003_02364928->rows[rowNumber - 1].unk_34 == 0) {
                Ov003Row_StopCountdown(rowNumber);
            }
        }
    }
}
