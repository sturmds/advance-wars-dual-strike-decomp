#include "types.h"
#include "sdk/spi_thread.h"
#include "sdk/os_arm7.h"

/* TRUE when a job waits in the SPI queue. Peeks only: the job stays queued. */
BOOL SPIi_CheckEntry(void) {
  OSMessage msg;

  return MsgQueue_PeekOldest_Arm7(&data_arm7_03809ab0.msgQ, &msg,
                                  OS_MESSAGE_NOBLOCK);
}
