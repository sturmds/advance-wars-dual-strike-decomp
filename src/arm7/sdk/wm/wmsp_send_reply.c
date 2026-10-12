#include "types.h"
#include "sdk/os_arm7.h"
#include "sdk/wmsp.h"

void func_arm7_037fdc04(int count);
int func_arm7_037fe1cc(int tag, u32 data, BOOL err);

void Wmsp_SendReply(void *reply)
{
    while (func_arm7_037fe1cc(PXI_FIFO_TAG_WM, (u32)reply, FALSE) < 0) {
        func_arm7_037fdc04(0x100);
    }
    Mutex_Release_Arm7(&data_arm7_027d813c);
}
