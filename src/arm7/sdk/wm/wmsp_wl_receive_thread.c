#include "types.h"
#include "sdk/os_arm7.h"
#include "sdk/wmsp.h"

void Wmsp_DispatchIndication(WlIndicationHeader* ind);

/* Thread: takes each message WL sends up. Indications (commands 0x80-0xff
   and 0x180-0x1ff) are handled here; replies go on to Wmsp_SendWlCommand. A
   NULL message ends the thread. */
void Wmsp_WlReceiveThread(void* arg) {
  WmspWork* work = func_arm7_027c05e0();
  WlIndicationHeader* message;
  u16 low;
  u16 high;

  while (TRUE) {
    MsgQueue_PopOldest_Arm7(&work->indicationQueue, (OSMessage*)&message,
                            OS_MESSAGE_BLOCK);
    if (message == NULL) {
      Thread_EndCurrent_Arm7();
      return;
    }
    low = message->command & 0xff;
    high = message->command & 0xff00;
    if ((high == 0 || high == 0x100) && (low & 0x80)) {
      Wmsp_DispatchIndication(message);
    } else {
      MsgQueue_PushBack_Arm7(&work->wlReplyQueue, message, OS_MESSAGE_BLOCK);
    }
  }
}
