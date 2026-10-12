#include "types.h"
#include "sdk/os.h"
#include "sdk/wmsp.h"

void func_arm7_037fc624(OSMutex *mutex);
void func_arm7_037fdc04(int count);

void *Wmsp_GetCallbackBuffer(void)
{
    func_arm7_037fc624(&data_arm7_027d813c);
    while (*WIRELESS_CALLBACK_LOCK_WORD & WIRELESS_CALLBACK_BUSY_BIT) {
        func_arm7_037fdc04(0x100);
    }
    *WIRELESS_CALLBACK_LOCK_WORD |= WIRELESS_CALLBACK_BUSY_BIT;
    return data_arm7_027d7c44.arm7Buf->replyBuf;
}
