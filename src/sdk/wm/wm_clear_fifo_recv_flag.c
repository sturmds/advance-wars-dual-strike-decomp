#include "sdk/wm.h"

/* Tells the ARM7 that the ARM9 has finished reading the last WM message from
 * the FIFO, so the ARM7 may reuse its buffer. */
void WmClearFifoRecvFlag(void) {
  if (*WIRELESS_CALLBACK_LOCK_WORD & WIRELESS_CALLBACK_BUSY_BIT) {
    *WIRELESS_CALLBACK_LOCK_WORD &= ~WIRELESS_CALLBACK_BUSY_BIT;
  }
}
