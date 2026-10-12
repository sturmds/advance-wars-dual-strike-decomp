#include "sdk/os_arm7.h"
#include "sdk/snd.h"

/* Blocks until the interval alarm or a command wakes the driver thread; returns the message. */
OSMessage SND_WaitForIntervalTimer(void)
{
    OSMessage msg;

    MsgQueue_PopOldest_Arm7(&data_arm7_03807b90, &msg, OS_MESSAGE_BLOCK);
    return msg;
}
