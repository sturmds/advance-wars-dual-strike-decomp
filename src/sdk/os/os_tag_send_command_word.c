#include "types.h"
#include "sdk/os.h"
#include "sdk/pxi.h"

/* PXI_SendWordByFifo: returns 0 once the word is queued. */

void OsTag_SendCommandWord(u32 command) {
  while (PXI_SendWordByFifo(FIFO_TAG_RESET, command << 8, 0) != 0) {
  }
}
