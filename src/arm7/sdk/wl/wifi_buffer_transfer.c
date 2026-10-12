#include "types.h"
#include "sdk/wl.h"

#define WIFI_LIST_LOCK_IRQS 0x01000000

u32 func_arm7_037fb654(u32 mask); /* ARM7 OS_DisableIrqMask */
u32 func_arm7_037fb690(u32 mask); /* ARM7 OS_EnableIrqMask */

/* Moves a buffer from one list to another. Returns the first error of the two list calls, or 0. */
int WifiBuffer_Transfer(WlBufferList *from, WlBufferList *to, WlBuffer *buffer)
{
    int error;
    u32 irqState;

    if (buffer->magic != WIFI_MSG_BUFFER_TAG) {
        return 1;
    }
    irqState = func_arm7_037fb654(WIFI_LIST_LOCK_IRQS);
    error = WifiBufList_Unlink(from, buffer);
    if (error == 0) {
        error = WifiBufList_PushTailIfLoose(to, buffer);
    }
    func_arm7_037fb690(irqState);
    return error;
}
