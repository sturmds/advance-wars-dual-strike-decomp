#include "sdk/os_arm7.h"
#include "sdk/snd.h"

BOOL func_arm7_037fc3ac(OSMessageQueue *queue, OSMessage *msg, int flags);

/* Blocks until the interval alarm or a command wakes the driver thread; returns the message. */
OSMessage SND_WaitForIntervalTimer(void)
{
    OSMessage msg;

    func_arm7_037fc3ac(&data_arm7_03807b90, &msg, OS_MESSAGE_BLOCK);
    return msg;
}
