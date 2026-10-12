#include "types.h"
#include "sdk/os_arm7.h"
#include "sdk/pxi.h"

extern u16 data_arm7_03807aac;

void func_arm7_037fe298(u32 tag, PXIFifoCallback callback);
void func_arm7_037fdca8(u32 tag, u32 data, int err);

/* The ARM7 half of OS_InitReset: registers the OS tag's PXI callback once.
 * Unlike the ARM9 side it does not wait for the other CPU. */
void func_arm7_037fdcf4(void)
{
    if (data_arm7_03807aac != 0) {
        return;
    }
    data_arm7_03807aac = 1;
    func_arm7_037fe298(FIFO_TAG_RESET, func_arm7_037fdca8);
}
