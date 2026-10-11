#include "types.h"
#include "sdk/os.h"

/* PXI_SendWordByFifo: returns 0 once the word is queued. */
int PXI_SendWordByFifo(u32 tag, u32 data, int err);

void OsTag_SendCommandWord(u32 command)
{
    while (PXI_SendWordByFifo(FIFO_TAG_RESET, command << 8, 0) != 0) {
    }
}
