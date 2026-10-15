#include "types.h"
#include "sdk/ctrdg.h"
#include "sdk/pxi.h"

/* PXI_SendWordByFifo: returns 0 once the word is queued. */

void Cartridge_SendWordUntilTaken(u32 word) {
  while (PXI_SendWordByFifo(FIFO_TAG_GBA_SLOT, word, 0) != 0) {
    SVC_WaitByLoop(1);
  }
}
