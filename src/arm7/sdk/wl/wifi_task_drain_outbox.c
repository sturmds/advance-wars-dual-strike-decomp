#include "types.h"
#include "sdk/os_arm7.h"
#include "sdk/wl.h"

/* Task 19: hands outbox buffers to the host, oldest first, until the host queue is full. */
void WifiTask_DrainOutbox(void) {
  WlBuffer* buffer = WIFI_DRIVER_STATE->outbox.first;

  while (buffer != WIFI_LIST_END) {
    if (!MsgQueue_PushBack_Arm7(WIFI_DRIVER_STATE->limits.toHost, buffer,
                                OS_MESSAGE_NOBLOCK)) {
      break;
    }
    WifiBufList_Unlink(&WIFI_DRIVER_STATE->outbox, buffer);
    buffer = WIFI_DRIVER_STATE->outbox.first;
  }
}
