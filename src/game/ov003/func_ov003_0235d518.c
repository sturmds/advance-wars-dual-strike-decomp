#include "types.h"
#include "game/ov003_unit.h"

extern u8 data_ov003_02364cfc[];
int func_ov003_0235da18(int rowNumber);

/* Byte from a table of 23 per group (index masked to 5 bits, out of range gives
 * entry 0). For a row: x1.5 in phase 6 while its countdown runs, halved while
 * unk_36 is set. Row 0 gets the table value as is. */
u8 func_ov003_0235d518(int rowNumber, int group, int index)
{
    u8 value;

    index &= 31;
    if (index >= 23) {
        index = 0;
    }
    value = data_ov003_02364cfc[group * 23 + index];
    if (rowNumber == 0) {
        return value;
    }
    if (func_ov003_0235da18(rowNumber) == 1) {
        if (data_ov003_02364928->rows[rowNumber - 1].unk_3b_0 == 6) {
            value = value * 3 >> 1;
        }
    }
    if (data_ov003_02364928->rows[rowNumber - 1].unk_36 != 0) {
        value = value >> 1;
    }
    return value;
}
