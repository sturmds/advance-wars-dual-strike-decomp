#include "types.h"
#include "sdk/os_arm7.h"
#include "sdk/wl.h"

BOOL func_arm7_037fc3ac(OSMessageQueue *queue, OSMessage *message, int flags); /* OS_ReceiveMessage */

/* Task handler: waits for a buffer from the host and queues the task that handles it. */
void WifiTask_WaitForHost(void)
{
    OSMessage message;

    func_arm7_037fc3ac(WIFI_DRIVER_STATE->limits.fromHost, &message, OS_MESSAGE_BLOCK);
    WifiHost_TakeInboxBuffer(&message);
    WifiTask_Schedule(3, 12);
}
