#include "types.h"
#include "sdk/os_arm7.h"
#include "sdk/wl.h"

/*
 * Hands a buffer to the host side. While the outbox holds older buffers, or
 * when the host queue is full, it joins the outbox and task 19 is queued.
 */
void WifiBuffer_GiveToHost(WlBufferList *list, WlBuffer *buffer)
{
    if (WIFI_DRIVER_STATE->outbox.count != 0) {
        WifiBuffer_Transfer(list, &WIFI_DRIVER_STATE->outbox, buffer);
        WifiTask_Schedule(2, 19);
    } else if (MsgQueue_PushBack_Arm7(WIFI_DRIVER_STATE->limits.toHost, buffer, OS_MESSAGE_NOBLOCK)) {
        WifiBufList_Unlink(list, buffer);
    } else {
        WifiBuffer_Transfer(list, &WIFI_DRIVER_STATE->outbox, buffer);
    }
}
