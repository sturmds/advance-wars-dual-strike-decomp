#include "sdk/os_arm7.h"
#include "sdk/snd.h"

BOOL func_arm7_037fc44c(OSMessageQueue *queue, OSMessage msg, int flags);

/* Debug print hook; points at an empty function in the retail build. */
extern void (*data_arm7_038077b4)(const char *format, ...);
/* "Failed sound alarm OS_SendMessage\n" */
extern const char data_arm7_038077c0[];

/* Handler of the interval alarm: tells the driver thread to run its periodic work. */
void SndDrv_OnIntervalAlarm(void *arg)
{
    if (!func_arm7_037fc44c(&data_arm7_03807b90, (OSMessage)SOUND_THREAD_MSG_TICK, OS_MESSAGE_NOBLOCK)) {
        data_arm7_038077b4(data_arm7_038077c0);
    }
}
