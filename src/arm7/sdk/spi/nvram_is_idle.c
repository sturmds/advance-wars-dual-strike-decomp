#include "types.h"
#include "sdk/nvram.h"


/* TRUE when no write or erase is running on the flash. */
BOOL NVRAM_IsIdle(void)
{
    u16 status;

    NVRAM_ReadStatus((u8 *)&status);
    if (status & NVRAM_STATUS_BUSY) {
        return FALSE;
    }
    return TRUE;
}
