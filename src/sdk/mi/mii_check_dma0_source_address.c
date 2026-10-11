#include "sdk/mi.h"

void OS_Terminate(void);

/* Halts if a channel 0 transfer would read the I/O registers or cartridge space. */
void MIi_CheckDma0SourceAddress(u32 dmaNo, u32 src, u32 size, u32 srcControl)
{
    u32 start;

    if (dmaNo != 0) {
        return;
    }
    start = src & ADDRESS_REGION_MASK;
    if (srcControl != 0) {
        if (srcControl == DMA_SRC_DECREMENT) {
            src -= size;
        }
    } else {
        src += size;
    }
    if (start == REGION_IO || start >= REGION_CARTRIDGE
        || (src & ADDRESS_REGION_MASK) == REGION_IO || (src & ADDRESS_REGION_MASK) >= REGION_CARTRIDGE) {
        OS_Terminate();
    }
}
