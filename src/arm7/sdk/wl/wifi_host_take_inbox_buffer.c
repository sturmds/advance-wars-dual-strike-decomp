#include "types.h"
#include "sdk/wl.h"

/* Files a buffer received from the host in the inbox and queues the task that reads it. */
void WifiHost_TakeInboxBuffer(void **message)
{
    if (*message != NULL) {
        WifiBufList_PushTail(&WIFI_DRIVER_STATE->inbox, *message);
        WifiTask_Schedule(2, 11);
    }
}
