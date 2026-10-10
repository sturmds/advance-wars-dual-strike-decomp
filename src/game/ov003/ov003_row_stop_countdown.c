#include "types.h"
#include "game/ov003_unit.h"

/* Clears the row's countdown (unk_34) and unk_32. */
void Ov003Row_StopCountdown(int rowNumber)
{
    data_ov003_02364928->rows[rowNumber - 1].unk_34 = 0;
    data_ov003_02364928->rows[rowNumber - 1].unk_32 = 0;
}
