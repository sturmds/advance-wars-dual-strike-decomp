#include "types.h"
#include "sdk/os_arm7.h"
#include "sdk/snd.h"

void SND_SendWakeupMessage(void) {
  MsgQueue_PushBack_Arm7(&data_arm7_03807b90,
                         (OSMessage)SOUND_THREAD_MSG_NEW_REQUESTS,
                         OS_MESSAGE_NOBLOCK);
}
