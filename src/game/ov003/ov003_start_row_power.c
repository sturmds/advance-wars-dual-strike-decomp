#include "types.h"
#include "game/ov003_row_status.h"

extern u16 data_ov003_02364cde[][2];

void Ov003_HealRowUnits(int row);
void func_ov003_0235db04(int row);
void Ov003Row_StopCountdown(int row);

/* Starts the row's power: sets the timer from the table, runs the start effect
 * of kind 1 (heal) and 7 (task), and calls Ov003Row_StopCountdown at once when
 * the timer is 0. */
void Ov003_StartRowPower(int row)
{
    OV003_ROW_STATUS(row).powerTimer = data_ov003_02364cde[OV003_ROW_STATUS(row).kind][0];
    switch (OV003_ROW_STATUS(row).kind) {
    case 1:
        Ov003_HealRowUnits(row);
        break;
    case 7:
        func_ov003_0235db04(row);
        break;
    }
    if (OV003_ROW_STATUS(row).powerTimer == 0) {
        Ov003Row_StopCountdown(row);
    }
}
