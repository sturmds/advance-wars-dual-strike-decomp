#include "sdk/os_arm7.h"
#include "sdk/snd.h"

/* Message queue init; the definition types the buffer as int. */
void func_arm7_037fd424(OSAlarm *alarm); /* clears the alarm's handler and tag */

#define SOUND_THREAD_MSG_SLOTS 8

extern OSMessage data_arm7_03807ba4[SOUND_THREAD_MSG_SLOTS]; /* buffer of data_arm7_03807b90 */

void SND_InitIntervalTimer(void)
{
    MsgQueue_SetUpRing_Arm7(&data_arm7_03807b90, data_arm7_03807ba4, SOUND_THREAD_MSG_SLOTS);
    func_arm7_037fd424(&data_arm7_03807bc4);
}
