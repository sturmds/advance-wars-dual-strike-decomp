#include "types.h"
#include "sdk/nvram.h"

void func_arm7_027d4884(void);

/* Polls the flash status until no write or erase is running, resetting the
 * flash whenever it reports a failed one. */
void WifiFlash_AwaitIdle(void)
{
    volatile u32 status;   /* needed to match: the original reloads it for each test */

    while (TRUE) {
        NVRAM_ReadStatus((u8 *)&status);
        if (status & NVRAM_STATUS_ERROR) {
            func_arm7_027d4884();
        } else if (!(status & NVRAM_STATUS_BUSY)) {
            break;
        }
    }
}
