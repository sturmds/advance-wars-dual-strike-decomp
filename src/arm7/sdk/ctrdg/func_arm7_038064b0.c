#include "types.h"
#include "sdk/ctrdg.h"

int func_arm7_037fe1cc(u32 tag, u32 data, int err);   /* PXI_SendWordByFifo: 0 once queued */
void func_arm7_037fdc04(u32 count);                   /* SVC_WaitByLoop through a veneer */

void func_arm7_038064b0(u32 word)
{
    while (func_arm7_037fe1cc(FIFO_TAG_GBA_SLOT, word, 0) != 0) {
        func_arm7_037fdc04(1);
    }
}
