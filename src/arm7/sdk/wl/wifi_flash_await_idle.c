#include "types.h"
#include "sdk/nvram.h"

/* Polls the flash status until no write or erase is running, resetting the
 * flash whenever it reports a failed one. */
void WifiFlash_AwaitIdle(void) {
  volatile u32
      status; /* needed to match: the original reloads it for each test */

  while (TRUE) {
    NVRAM_ReadStatus((u8*)&status);
    if (status & NVRAM_STATUS_ERROR) {
      NVRAM_SoftwareReset();
    } else if (!(status & NVRAM_STATUS_BUSY)) {
      break;
    }
  }
}
