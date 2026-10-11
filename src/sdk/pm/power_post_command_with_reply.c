#include "sdk/pm.h"

extern PowerCommandState gPMWork;

BOOL Power_TryTakeCommandSlot(void);
void PMi_SendPxiData(u32 data);

/* Sends a PM command as two PXI words; its reply calls callback. Returns 1 when another command is in flight. */
int Power_PostCommandWithReply(u32 command, PMCallback callback, u32 *arg)
{
    if (!Power_TryTakeCommandSlot()) {
        return 1;
    }
    gPMWork.callback = callback;
    gPMWork.arg = arg;
    PMi_SendPxiData(0x02006300 | ((command >> 16) & 0xff));
    PMi_SendPxiData((command & 0xffff) | 0x01010000);
    return 0;
}
