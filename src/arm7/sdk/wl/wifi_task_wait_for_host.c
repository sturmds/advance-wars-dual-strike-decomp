#include "types.h"
#include "sdk/os_arm7.h"
#include "sdk/wl.h"

/* Task handler: waits for a buffer from the host and queues the task that handles it. */
void WifiTask_WaitForHost(void) {
  OSMessage message;

  MsgQueue_PopOldest_Arm7(WIFI_DRIVER_STATE->limits.fromHost, &message,
                          OS_MESSAGE_BLOCK);
  WifiHost_TakeInboxBuffer(&message);
  WifiTask_Schedule(3, 12);
}
