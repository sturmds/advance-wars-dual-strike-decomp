#include "types.h"
#include "sdk/os.h"

/* PXI_SendWordByFifo: returns 0 once the word is queued. */
int func_02110f88(u32 tag, u32 data, int err);

void OsTag_SendCommandWord(u32 command)
{
    while (func_02110f88(FIFO_TAG_RESET, command << 8, 0) != 0) {
    }
}
