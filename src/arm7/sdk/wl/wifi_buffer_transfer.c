#include "types.h"
#include "sdk/wl.h"
#include "sdk/os_arm7.h"

#define WIFI_LIST_LOCK_IRQS 0x01000000

/* Moves a buffer from one list to another. Returns the first error of the two list calls, or 0. */
int WifiBuffer_Transfer(WlBufferList *from, WlBufferList *to, WlBuffer *buffer)
{
    int error;
    u32 irqState;

    if (buffer->magic != WIFI_MSG_BUFFER_TAG) {
        return 1;
    }
    irqState = Irq_ClearEnableBits_Arm7(WIFI_LIST_LOCK_IRQS);
    error = WifiBufList_Unlink(from, buffer);
    if (error == 0) {
        error = WifiBufList_PushTailIfLoose(to, buffer);
    }
    Irq_SetEnableBits_Arm7(irqState);
    return error;
}
