#include "types.h"
#include "sdk/ctrdg.h"

/* PXI_SendWordByFifo: returns 0 once the word is queued. */
int PXI_SendWordByFifo(u32 tag, u32 data, int err);

void Cartridge_SendWordUntilTaken(u32 word) {
  while (PXI_SendWordByFifo(FIFO_TAG_GBA_SLOT, word, 0) != 0) {
    SVC_WaitByLoop(1);
  }
}
