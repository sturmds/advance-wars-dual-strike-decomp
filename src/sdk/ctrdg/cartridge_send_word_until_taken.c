#include "types.h"
#include "sdk/ctrdg.h"

/* PXI_SendWordByFifo: returns 0 once the word is queued. */
int func_02110f88(u32 tag, u32 data, int err);

void Cartridge_SendWordUntilTaken(u32 word)
{
    while (func_02110f88(FIFO_TAG_GBA_SLOT, word, 0) != 0) {
        WaitByLoop(1);
    }
}
