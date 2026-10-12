#include "types.h"
#include "sdk/nvram.h"

/* TRUE when the flash is idle and its write enable latch is set, so a write
 * or erase instruction will be taken. */
BOOL NVRAM_IsWriteEnabled(void) {
  u16 status;

  NVRAM_ReadStatus((u8*)&status);
  if (status & NVRAM_STATUS_BUSY) {
    return FALSE;
  }
  if (status & NVRAM_STATUS_WRITE_ENABLE) {
    return TRUE;
  }
  return FALSE;
}
