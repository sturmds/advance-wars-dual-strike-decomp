#include "types.h"
#include "game/ov003_row_status.h"

/* row is 1 to 5. */
int Ov003_IsRowPowerActive(int row)
{
    return OV003_ROW_STATUS(row).powerTimer != 0;
}
